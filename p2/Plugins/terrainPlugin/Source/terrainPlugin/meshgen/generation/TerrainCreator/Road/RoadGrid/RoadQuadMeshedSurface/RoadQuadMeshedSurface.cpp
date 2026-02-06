#include "RoadQuadMeshedSurface.h"
#include "GameCore/util/FVectorUtil.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"


RoadQuadMeshedSurface::RoadQuadMeshedSurface(){

}

RoadQuadMeshedSurface::~RoadQuadMeshedSurface(){

}

RoadQuadMeshedSurface::RoadQuadMeshedSurface(const RoadQuadMeshedSurface &other){
    if(this != &other){
        *this = other;
    }
}

RoadQuadMeshedSurface &RoadQuadMeshedSurface::operator=(const RoadQuadMeshedSurface &other){
    if(this != &other){

    }
    return *this;
}


void RoadQuadMeshedSurface::SetTemporaryTerrainCreatorReference(terrainCreator *creator){
    temporaryReference = creator;
}

void RoadQuadMeshedSurface::ResetTemporaryTerrainCreatorReference(){
    temporaryReference = nullptr;
}



void RoadQuadMeshedSurface::Init(
    RoadQuad &quad, 
    terrainCreator *creator,
    float widthOfInsideStep
){
    if(creator){
        SetTemporaryTerrainCreatorReference(creator);
        Init(quad, widthOfInsideStep);
        ResetTemporaryTerrainCreatorReference();
    }
}


void RoadQuadMeshedSurface::Init(
    RoadQuad &quad, 
    float widthOfInsideStep
){
    widthOfInsideStep = std::max(widthOfInsideStep, 100.0f);
    TArray<FVector> &inner = quad.GetInnerCircle();
    //Init(inner, widthOfInsideStep);
    RasterizePolygonEdgeTo(
        inner,
        widthOfInsideStep
    );
    UpdateHeightOnAllFrames();
}

void RoadQuadMeshedSurface::UpdateHeightOnAllFrames(){
    if(temporaryReference){
        for (int i = 0; i < surface.Num(); i++){
            TArray<FMeshedSurfaceFrame> &current = surface[i];
            for (int j = 0; j < current.Num(); j++){
                FMeshedSurfaceFrame &currentFrame = current[j];
                currentFrame.UpdateHeightOnVertecies(temporaryReference);
            }
        }
        meshedGrid.UpdateHeights(temporaryReference);
    }
}




bool RoadQuadMeshedSurface::AreaOfPolygonAroundCenterExceedsLimit(
    const TArray<FVector> &polygon,
    TArray<FVector> &outScaledDown,
    float scaleDownWidth //inward push in cm
){
    //als quadrat eben. X^2. of X, * 2, because scaled from every side. (also the opposite side.)
    float minAreaToReach = scaleDownWidth * scaleDownWidth * 2;

    FVector center = FVectorUtil::calculateCenter(polygon);
    if(AreaOfPolygonAroundCenter(polygon, center) > minAreaToReach){
        ScaleDownByWidth(polygon, outScaledDown, scaleDownWidth, center);
        return true; //still above limit
    }
    return false;
}

float RoadQuadMeshedSurface::AreaOfPolygonAroundCenter(
    const TArray<FVector> &polygon,
    const FVector &center
){
    
    float area = 0.0f;

    int num = polygon.Num();
    if(num > 1){ //lower is no triangle, at least 2.
        for (int i = 0; i < polygon.Num(); i++)
        {
            int next = (i + 1) % polygon.Num();

            area += FVectorUtil::AreaTriangle(
                polygon[i],
                polygon[next],
                center
            );
        }
    }
    DebugHelper::logMessage(FString::Printf(TEXT("RoadQuadMeshedSurface::AreaOfPolygonAroundCenter %.2f"), area));
    return area;
}






// scale down operation towards center
void RoadQuadMeshedSurface::ScaleDownByWidth(
    const TArray<FVector> &circle,
    TArray<FVector> &OutInnerCircle,
    float width
){
    FVector center = FVectorUtil::calculateCenter(circle);
    ScaleDownByWidth(circle, OutInnerCircle, width, center);
}

void RoadQuadMeshedSurface::ScaleDownByWidth(
    const TArray<FVector> &circle,
    TArray<FVector> &OutInnerCircle,
    float width,
    const FVector &center
){
    if(circle.Num() > 0){
        OutInnerCircle.SetNum(circle.Num());
        FVector normal;
        for (int i = 0; i < circle.Num(); i++){
            const FVector &current = circle[i];
            normal = (center - current);// AB = B - A
            normal.Z = 0.0f;
            normal = normal.GetSafeNormal(); 

            OutInnerCircle[i] = current + normal * width; //update
        }
    }
}





void RoadQuadMeshedSurface::AppendMeshedSurface(MeshData &data){
    for (int i = 0; i < surface.Num(); i++){
        TArray<FMeshedSurfaceFrame> &current = surface[i];
        for (int j = 0; j < current.Num(); j++){
            FMeshedSurfaceFrame &currentFrame = current[j];
            currentFrame.AppendMeshedSurface(data);
        }
    }
    meshedGrid.AppendMeshedSurface(data);
}














/// ------- NEUE IDEE --------
//hier soll der spline so bearbeitet werden dass der increase detail immer
//auf x und y gecapped wird, der spline wird also wie rasterisiert
void RoadQuadMeshedSurface::RasterizePolygonEdgeTo(
    TArray<FVector> &polygon,
    float widthOfInsideStep
){
    TArray<FVector> rasterized = polygon;
    RasterizeVerteciesTo(rasterized, widthOfInsideStep); //roght off rasterized edge
    Draw(rasterized);

    //create grid data
    meshedGrid.Init(rasterized, widthOfInsideStep);

    //create surface frames
    surface.SetNum(1);
    TArray<FMeshedSurfaceFrame> &framesRef = surface[0];

    for (int i = 0; i < polygon.Num(); i++)
    {
        
        int next = (i + 1) % polygon.Num();
        FVector &v3 = polygon[i];
        FVector &v0 = polygon[next];
        
        FVector &v1 = rasterized[next];
        FVector &v2 = rasterized[i];

        FMeshedSurfaceFrame frame(v0, v1, v2, v3);
        framesRef.Add(frame);


    }
}

void RoadQuadMeshedSurface::Draw(TArray<FVector> &rasterized){

    float offset = 60.0f;
    for (int i = 0; i < rasterized.Num(); i++)
    {
        int next = (i + 1) % rasterized.Num();
        FVector v0 = rasterized[i];
        FVector v1 = rasterized[next];

        FColor color = (i % 2 == 0) ? FColor::Red : FColor::Blue;
        Draw(v0, v1, offset, color);
        Draw(v0, v1, offset * 2, color);
        Draw(v0, v1, offset * 3, color);
    }
}

void RoadQuadMeshedSurface::Draw(FVector a, FVector b, float offset, FColor color){
    UWorld *world = AworldLevelBase::GetWorldPointer();
    a.Z += offset;
    b.Z += offset;
    DebugHelper::showLineBetween(world, a, b, color, 1000.0f);
}



void RoadQuadMeshedSurface::RasterizeVerteciesTo(
    TArray<FVector> &polygon,
    float widthOfInsideStep
){
    for (int i = 0; i < polygon.Num(); i++){
        FVector &current = polygon[i];
        FVector &next = polygon[(i + 1) % polygon.Num()];
        FVector connect = next - current; //AB = B - A
        FVector normal(connect.Y * -1.0f, connect.X, 0.0f);

        FVector modLower = ToModCoordinate(current, widthOfInsideStep, -1);
        FVector modHigher = ToModCoordinate(current, widthOfInsideStep, 1);

        //stepping coordinate must be always right off, inside the polygon
        polygon[i] = ChooseRightOffCoordinateRelativeTo(modLower, modHigher, normal, current);
    }
}


//refernce is only temporary by given coordinates
FVector& RoadQuadMeshedSurface::ChooseRightOffCoordinateRelativeTo(
    FVector &modLower,
    FVector &modHigher,
    FVector &normal,
    FVector &relative
){
    FVector modLowerRelative = modLower - relative; // AB = B - A
    //FVector modHigherRelative = modHigher - relative;

    //stepping coordinate must be always right off, inside the polygon
    if(FVector::DotProduct(normal, modLowerRelative) <= 0.0f){
        //return modHigher;
        return modLower;
    }
    //return modLower;
    return modHigher;
}

FVector RoadQuadMeshedSurface::ToModCoordinate(FVector &pos, int mod, int dir){
    int x = pos.X;
    int y = pos.Y;
    x += dir * (x % mod);
    y += dir * (y % mod);
    return FVector(x, y, pos.Z);
}