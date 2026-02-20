#include "RoadQuadMeshedSurface.h"
#include "GameCore/util/FVectorUtil.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "terrainPluginBase/BaseTerrainInterface/bezier/rasterizer/CurveRasterizer.h"

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
        edge = other.edge;
        meshedGrid = other.meshedGrid;
    }
    return *this;
}


void RoadQuadMeshedSurface::SetTemporaryTerrainCreatorReference(TerrainInterfaceBase *creator){
    temporaryReference = creator;
}

void RoadQuadMeshedSurface::ResetTemporaryTerrainCreatorReference(){
    temporaryReference = nullptr;
}



void RoadQuadMeshedSurface::Init(
    RoadQuad &quad, 
    TerrainInterfaceBase *creator,
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
        //update outer frames
        for (int i = 0; i < edge.Num(); i++){
            FMeshedSurfaceFrame &currentFrame = edge[i];
            currentFrame.UpdateHeightOnVertecies(temporaryReference);
        }

        //update grid 
        meshedGrid.UpdateHeights(temporaryReference);
    }
}







void RoadQuadMeshedSurface::AppendMeshedSurface(MeshData &data){
    for (int i = 0; i < edge.Num(); i++){
        FMeshedSurfaceFrame &currentFrame = edge[i];
        currentFrame.AppendMeshedSurface(data);
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

    CurveRasterizer rasterizer;
    rasterizer.RasterizeVerteciesTo(rasterized, widthOfInsideStep); // roght off rasterized edge
    Draw(rasterized);

    //create grid data
    meshedGrid.Init(rasterized, widthOfInsideStep);

    //create surface frames for edge
    for (int i = 0; i < polygon.Num(); i++)
    {
        
        int next = (i + 1) % polygon.Num();
        FVector &v3 = polygon[i];
        FVector &v0 = polygon[next];
        
        FVector &v1 = rasterized[next];
        FVector &v2 = rasterized[i];

        FMeshedSurfaceFrame frame(v0, v1, v2, v3);
        edge.Add(frame);


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

#include "GameCore/world/worldLevelBase.h"
void RoadQuadMeshedSurface::Draw(FVector a, FVector b, float offset, FColor color){
    UWorld *world = AworldLevelBase::GetWorldPointer();
    a.Z += offset;
    b.Z += offset;
    DebugHelper::showLineBetween(world, a, b, color, 1000.0f);
}
