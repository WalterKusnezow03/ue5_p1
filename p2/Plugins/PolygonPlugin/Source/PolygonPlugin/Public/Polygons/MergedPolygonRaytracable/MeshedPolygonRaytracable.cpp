#include "MeshedPolygonRaytracable.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "PolygonPlugin/Public/Polygons/rasterizer/CurveRasterizer.h"


void FMeshedPolygonRaytracable::CreateOrClearViewGrid(){
    TCreateOrClearGrid<float>(viewGrid, viewGridClearedValue);



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
    int asInt = FMath::CeilToInt(angle * 1.1f);
    TraceCone(pos, dir, angle, asInt);
}

void FMeshedPolygonRaytracable::TraceCone(const FVector &pos, const FVector2D &dir, float angle, float rays){
    int x, y = 0;
    ToIndexRaw(pos, x, y);
    TraceCone(x, y, dir, angle, rays);
}

void FMeshedPolygonRaytracable::TraceCone(int x, int y, const FVector2D &dir, float angle, float rays){

    float base = FMath::Atan2(dir.Y, dir.X);
    float half = FMath::DegreesToRadians(angle * 0.5f);

    TArray<FIntPoint> hits;
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
    //DebugHelper::logMessage(logMessage);

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedPolygonRaytracable::TraceCone Index Raw %d %d, vision Angle %.2f hits %d: %s"), 
            x, y, angle, hits.Num(),
            *MakeString(hits)
        )
    );

    /*
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



    // draw all lines
        // fill all lines
    FlagPositionsFromPolygon(hits, x, y);

    // fill gaps
    FlagBetweenSpace(viewGridTrueValue);

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedPolygonRaytracable::ViewGridAsString %s"),
            *ViewGridAsString()
        )
    );
}

FString FMeshedPolygonRaytracable::MakeString(const TArray<FIntPoint> &hits){
    FString result = "hits: ";
    for (int i = 0; i < hits.Num(); i++){
        FString s = "(" + hits[i].ToString() + ")";
        result += s;
    }
    return result;
}

bool FMeshedPolygonRaytracable::Trace(int x, int y, const FVector2D &dir, FIntPoint &outHit){
    //trace against polygons
    if(edgeSet.RayIntersectPolygons(x, y, dir, outHit)){
        return true;
    }


    if(boundHull.InsideHull(x,y)){
        //if not found
        //trace against edges
        float t = FLT_MAX;
        if(boundHull.RayIntersectPolygon(x,y,dir,outHit, t)){
            return true;
        }
    }else{
        //far hull
        float t = 0.0f;
        if(boundHull.RayIntersectPolygonFarHit(x,y,dir,outHit, t)){
            return true;
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

void FMeshedPolygonRaytracable::FlagPositionsFromPolygon(
    TArray<FIntPoint> &hits, //not rasterized properly yet
    int x,
    int y
){
    FIntPoint point(x, y);
    FlagPositionsFromPolygon(hits, point);
}

void FMeshedPolygonRaytracable::FlagPositionsFromPolygon(
    TArray<FIntPoint> &hits, //not rasterized properly yet
    const FIntPoint &start
){
    hits.Insert(start, 0);
    hits.Add(start);
    FlagPositionsFromPolygon(hits);
}

void FMeshedPolygonRaytracable::FlagPositionsFromPolygon(
    TArray<FIntPoint> &hits
){
    FString message = FString::Printf(TEXT("FMeshedPolygonRaytracable::Rasterized from %d"), hits.Num());
    CurveRasterizer rasterizer;
    rasterizer.RasterizeVerteciesIntBrensenham(hits);
    message += FString::Printf(TEXT(" to %d"), hits.Num());
    DebugHelper::logMessage(message);

    for (int i = 0; i < hits.Num(); i++){
        FlagPositon(hits[i]);
    }
}



void FMeshedPolygonRaytracable::FlagPositon(const FIntPoint &pos){
    TOverrideValue<float>(viewGrid, pos.X, pos.Y, viewGridTrueValue);
}








void FMeshedPolygonRaytracable::FlagBetweenSpace(
    float value
){
    for (int i = 0; i < viewGrid.Num(); i++){
        TArray<float> &column = viewGrid[i];
        FlagBetweenSpace(column, value);
    }
}

void FMeshedPolygonRaytracable::FlagBetweenSpace(TArray<float> &flagBuffer, float value){
    int start = -1;
    int end = -1;
    bool startFound = false;
    bool endFound = false;

    for (int i = 0; i < flagBuffer.Num(); i++)
    {
        bool current = flagBuffer[i] != viewGridClearedValue;
        if(!startFound && current){
            startFound = current;
            start = i;
            // reset copy
            current = false;
        }
        if(startFound && !endFound && current){
            end = i;
            //copy
            FlagBetweenSpace(flagBuffer, start, end, value);

            start = -1;
            end = -1;
            startFound = false;
            endFound = false;
        }
    }
}


void FMeshedPolygonRaytracable::FlagBetweenSpace(TArray<float> &flagBuffer, int i, int j, float value){
    i = std::max(i, 0);
    j = std::min(j, flagBuffer.Num());
    for (int k = i; k < j; k++){
        flagBuffer[k] = value;
    }
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