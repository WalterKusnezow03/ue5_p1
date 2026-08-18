#include "MeshedPolygonRaytracable.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "PolygonPlugin/Public/Polygons/rasterizer/CurveRasterizer.h"
#include "PolygonPlugin/Public/GridBase/Operator/ConvolutionOperatorGauss.h"



void FMeshedPolygonRaytracable::CreateOrClearViewGrid(){
    TCreateOrClearGrid<float>(viewGrid, viewGridClearedValue);

    RecreateBoundHull();
}

void FMeshedPolygonRaytracable::RecreateBoundHull(){
    int x = sizeX();
    int y = sizeY();

    /*
    1-->2
    |   |
    0<--3
    */
    boundHull.Clear();
    boundHull.AddPosition(0, 0);
    boundHull.AddPosition(x, 0);
    boundHull.AddPosition(x, y);
    boundHull.AddPosition(0, y);
}




bool FMeshedPolygonRaytracable::ViewGridValid(){
    return TGridIsSize(sizeX(), sizeY(), viewGrid);
}

void FMeshedPolygonRaytracable::TraceCone(const FVector &pos, const FVector2D &dir, float angle){
    TraceConeOnGrid(pos, dir, angle, viewGrid);
}

void FMeshedPolygonRaytracable::TraceConeOnGrid(
    const FVector &pos, 
    const FVector2D &dir, 
    float angle,
    TArray<TArray<float>> &grid //expects grid to be same size and space as this polygon
){
    int asInt = FMath::CeilToInt(angle * 1.1f);
    TraceConeOnGrid(pos, dir, angle, asInt, grid);
}


void FMeshedPolygonRaytracable::TraceConeOnGrid(
    const FVector &pos, const FVector2D &dir, float angle, float rays,
    TArray<TArray<float>> &grid //expects grid to be same size and space as this polygon
){
    int x = 0, y = 0;
    ToIndexRaw(pos, x, y);
    TraceConeOnGrid(x, y, dir, angle, rays, grid, true);
}

void FMeshedPolygonRaytracable::TraceConeOnGridBetweenDirections(
    const FVector &pos,
    const FVector2D &limitA,
    const FVector2D &limitB,
    TArray<TArray<float>> &grid // expects grid to be same size and space as this polygon
){
    FVector2D A = limitA.GetSafeNormal();
    FVector2D B = limitB.GetSafeNormal();

    float Dot = FVector2D::DotProduct(A, B);
    float Cross = A.X * B.Y - A.Y * B.X;

    float SignedAngleDegrees = FMath::RadiansToDegrees(FMath::Atan2(Cross, Dot));
    int rays = SignedAngleDegrees;

    int x = 0, y = 0;
    ToIndexRaw(pos, x, y);
    TraceConeOnGrid(x, y, A, SignedAngleDegrees, rays, grid, false);
}







void FMeshedPolygonRaytracable::TraceConeOnGrid(
    int x, int y, const FVector2D &dir, float angle, float rays,
    TArray<TArray<float>> &grid,
    bool dirIsCentered
){
    TArray<FIntPoint> hits;
    // bool dirIsCentered: true
    TraceConeCollectHits(x, y, dir, angle, rays, hits, dirIsCentered);

    float base = FMath::Atan2(dir.Y, dir.X);
    float half = FMath::DegreesToRadians(angle * 0.5f);

    
    FString logMessage = "FMeshedPolygonRaytracable::TraceCone rays";
    for (int i = 0; i < rays; i++){
        float t = (rays == 1) ? 0.5f : (float)i / (rays - 1);
        float a = base + FMath::Lerp(-half, half, t);
        FVector2D rayDir(FMath::Cos(a), FMath::Sin(a));

        logMessage += "(" + rayDir.ToString() + ")";

        FIntPoint outHit;
        if(Trace(x, y, rayDir, outHit)){
            hits.Add(outHit);
        }
    }
    

    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedPolygonRaytracable::TraceCone Index Raw %d %d, vision Angle %.2f hits %d: %s"), 
            x, y, angle, hits.Num(),
            *MakeString(hits)
        )
    );*/

    /*
    Counter clockwise order of rays!

    FMeshedPolygonRaytracable::TraceCone 
    rays
    (X=0.707 Y=-0.707)
    (X=0.819 Y=-0.574)
    (X=0.906 Y=-0.423)
    (X=0.966 Y=-0.259)
    (X=0.996 Y=-0.087)
    (X=0.996 Y=0.087)
    (X=0.966 Y=0.259)
    (X=0.906 Y=0.423)
    (X=0.819 Y=0.574)
    (X=0.707 Y=0.707)
    FMeshedPolygonRaytracable::TraceCone 
    Index Raw 42 67, vision Angle 90.00 hits 10: hits: 
    (X=109 Y=0)
    (X=137 Y=0)
    (X=142 Y=20)
    (X=141 Y=40)
    (X=141 Y=58)
    (X=141 Y=75)
    (X=141 Y=93)
    (X=142 Y=113)
    (X=142 Y=137)
    (X=116 Y=141)
    */
    FIntPoint startingPoint(x,y);
    //alle rays zeichnen, hinreichend um flächendeckend zu malen bei genug rays
    DrawLineFromPositionToHits(startingPoint, hits, grid);

    //polygon am cone start schliessen
    InjectStartingPointAtFrontAndEnd(hits, startingPoint);

    // draw all lines
    // fill all lines
    FlagPositionsFromPolygon(hits, grid);
}

void FMeshedPolygonRaytracable::TraceConeCollectHits(
    int x, int y, 
    const FVector2D &dir, 
    float angle, 
    float rays,
    TArray<FIntPoint> &hits,
    bool dirIsCentered
){
    float base = FMath::Atan2(dir.Y, dir.X);
    float angleRad = FMath::DegreesToRadians(angle);
    float half = angleRad * 0.5f;

    
    FString logMessage = "FMeshedPolygonRaytracable::TraceCone rays";
    for (int i = 0; i < rays; i++){
        float t = (rays == 1) ? 0.5f : (float)i / (rays - 1);
        
        
        float lerped = 0.0f;
        if(dirIsCentered){
            lerped = FMath::Lerp(-half, half, t);
        }else{
            lerped = FMath::Lerp(0, angleRad, t);
        }

        float theta = base + lerped;

        FVector2D rayDir(FMath::Cos(theta), FMath::Sin(theta));

        logMessage += "(" + rayDir.ToString() + ")";

        FIntPoint outHit;
        if(Trace(x, y, rayDir, outHit)){
            hits.Add(outHit);
        }
    }
}



    







void FMeshedPolygonRaytracable::InjectStartingPointAtFrontAndEnd(
    TArray<FIntPoint> &hits, //not rasterized properly yet
    const FIntPoint &start
){
    hits.Insert(start, 0);
    hits.Add(start);
}

FString FMeshedPolygonRaytracable::MakeString(const TArray<FIntPoint> &hits){
    FString result = "hits: ";
    for (int i = 0; i < hits.Num(); i++){
        FString s = "(" + hits[i].ToString() + ")";
        result += s;
    }
    return result;
}

bool FMeshedPolygonRaytracable::Trace(
    int x, 
    int y, 
    const FVector2D &dir, 
    FIntPoint &outHit
){
    return Trace(x, y, dir, outHit, false);
}

bool FMeshedPolygonRaytracable::Trace(
    int x, 
    int y, 
    const FVector2D &dir, 
    FIntPoint &outHit,
    bool ignoreBounds
){
    float tIgnored = 0.0f;
    return Trace(x, y, dir, outHit, ignoreBounds, tIgnored);
}

bool FMeshedPolygonRaytracable::Trace(
    int x, 
    int y, 
    const FVector2D &dir, 
    FIntPoint &outHit,
    bool ignoreBounds,
    float &outT
){
    //trace against polygons
    if(edgeSet.RayIntersectPolygons(x, y, dir, outHit, outT)){
        return true;
    }

    if(!ignoreBounds){
        if(boundHull.InsideHull(x,y)){
            //if not found
            //trace against edges
            float t = FLT_MAX;
            if(boundHull.RayIntersectPolygon(x,y,dir,outHit, t)){
                outT = t;
                return true;
            }
        }else{
            //far hull
            float t = 0.0f;
            if(boundHull.RayIntersectPolygonFarHit(x,y,dir,outHit, t)){
                outT = t;
                return true;
            }
        }
    }

    
    return false;
}


void FMeshedPolygonRaytracable::BresenhamLineAppend(
    const FIntPoint &Start, 
    const FIntPoint &End,
    TArray<FIntPoint> &outPoints
){
    //outPoints.Empty(); //dont Empty

    int x0 = Start.X;
    int y0 = Start.Y;
    int x1 = End.X;
    int y1 = End.Y;

    int dx = FMath::Abs(x1 - x0);
    int dy = FMath::Abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    while (true){
        outPoints.Add(FIntPoint(x0, y0));
        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy){
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx){
            err += dx;
            y0 += sy;
        }
    }
}



void FMeshedPolygonRaytracable::DrawLineFromPositionToHits(
    FIntPoint &start,
    TArray<FIntPoint> &hits,
    TArray<TArray<float>> &grid
){
    TArray<FIntPoint> tmpList;

    CurveRasterizer rasterizer;
    for (int i = 0; i < hits.Num(); i++){
        tmpList.Empty();
        tmpList.Add(start);
        tmpList.Add(hits[i]);
        rasterizer.RasterizeVerteciesIntBrensenham(tmpList);

        for (int j = 0; j < tmpList.Num(); j++){
            FlagPositon(tmpList[j], grid);
        }
    }
}


void FMeshedPolygonRaytracable::FlagPositionsFromPolygon(
    TArray<FIntPoint> &hits,
    TArray<TArray<float>> &grid
){
    FString message = FString::Printf(TEXT("FMeshedPolygonRaytracable::Rasterized from %d"), hits.Num());
    CurveRasterizer rasterizer;
    rasterizer.RasterizeVerteciesIntBrensenham(hits);
    message += FString::Printf(TEXT(" to %d"), hits.Num());
    DebugHelper::logMessage(message);

    for (int i = 0; i < hits.Num(); i++){
        FlagPositon(hits[i], grid);
    }
}



void FMeshedPolygonRaytracable::FlagPositon(const FIntPoint &pos, TArray<TArray<float>> &grid){
    TOverrideValue<float>(grid, pos.X, pos.Y, viewGridTrueValue);
}














FString FMeshedPolygonRaytracable::ViewGridAsString(){
    //todo!
    int count = 0;
    for (int i = 0; i < viewGrid.Num(); i++){
        TArray<float> &current = viewGrid[i];
        
        for (int j = 0; j < current.Num(); j++){
            if(current[j] > 0.0f){
                count++;
            }
        }
    }
    FString result = FString::Printf(TEXT("View grid count: %d"), count);
    return result;
}


    
void FMeshedPolygonRaytracable::ApplyGaussViewGrid(){
    int size = 3; //-3 1 +3
    float sigma = 1.0f; //1.0f
    ApplyGaussViewGrid(size, sigma);
}

void FMeshedPolygonRaytracable::ApplyGaussViewGrid(int sizeMask, float sigma){
    if(ViewGridValid()){
        ConvolutionOperatorGauss gauss(sigma, sizeMask);
        gauss.ApplyOperator(viewGrid);
    }
}



void FMeshedPolygonRaytracable::AppendAsBinary(
    TArray<uint8> &buffer
){
    if(IsValid()){
        FMeshedPolygon::AppendAsBinary(buffer);

        if(!ViewGridValid()){
            CreateOrClearViewGrid();
        }
        


        TemplateBufferStorageInterface::TAppendGrid<float>(viewGrid, buffer);
    }
}



bool FMeshedPolygonRaytracable::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *& Ptr //reference to a pointer. Pointer by reference.
){
    if(FMeshedPolygon::LoadFromBinary(buffer, Ptr)){
        /*if(TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            return false;
        }*/
        TemplateBufferStorageInterface::TLoadGrid<float>(viewGrid, Ptr);
        return true;
    }
    return false;
}


bool FMeshedPolygonRaytracable::IsVisible(const FVector &a, const FVector &b){
    int xA, yA = 0;
    ToIndexRaw(a, xA, yA);
    FVector2D dir = MakeDir(a, b);
    float maxT = FVector::Dist(a, b);

    FIntPoint outHit;
    float t = 0.0f;
    if(Trace(xA, yA, dir, outHit, true, t)){
        //if t is in range [0,1] a hit was detected before the position was
        //reached
        if(t >= 0.0f){
            //if(t < 0.9f){
            if(t < maxT + 0.01f){
                return false;
            }
        }
    }
    return true;
}

FVector2D FMeshedPolygonRaytracable::MakeDir(const FVector &v0, const FVector &v1){
    FVector dir = v1 - v0; //AB = B - A
    dir.Z = 0.0f;
    dir = dir.GetSafeNormal();

    FVector2D dir2D(dir.X, dir.Y);
    dir2D = dir2D.GetSafeNormal();
    return dir2D;
}




void FMeshedPolygonRaytracable::ResizeGrid(int x, int y){
    if(x > 0 && y > 0){
        FMeshedPolygon::ResizeGrid(x, y);

        TResizeGrid<float>(viewGrid, viewGridClearedValue, x, y);

        RecreateBoundHull();
    }
}