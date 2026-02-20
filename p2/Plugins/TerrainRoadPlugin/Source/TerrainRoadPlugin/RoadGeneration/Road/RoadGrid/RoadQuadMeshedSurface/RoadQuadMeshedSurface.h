#pragma once

#include "CoreMinimal.h"


#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadQuadMeshedSurface/MeshedSurfaceFrame.h"
#include "TerrainRoadPlugin/RoadGeneration/Road/RoadGrid/RoadQuadMeshedSurface/MeshedSurfaceGrid.h"

class RoadQuad;
class TerrainInterfaceBase;
class MeshData;

//will store a set of quads from the road quad and terrain creator heights
//inidididual quads will be assigned for buildings
//search functions for a convex area will be searchable
class TERRAINROADPLUGIN_API RoadQuadMeshedSurface {

public:
    RoadQuadMeshedSurface();
    ~RoadQuadMeshedSurface();

    RoadQuadMeshedSurface(const RoadQuadMeshedSurface &other);
    RoadQuadMeshedSurface &operator=(const RoadQuadMeshedSurface &other);

    void Init(
        RoadQuad &quad, 
        TerrainInterfaceBase *creator,
        float widthOfInsideStep
    );


    //debug
    void AppendMeshedSurface(MeshData &data);

    FMeshedSurfaceGrid *GetFMeshedSurfaceGridPointer(){
        return &meshedGrid;
    }

private:
    void Init(
        RoadQuad &quad,
        float widthOfInsideStep
    );

    void SetTemporaryTerrainCreatorReference(TerrainInterfaceBase *creator);
    void ResetTemporaryTerrainCreatorReference();

    TerrainInterfaceBase *temporaryReference = nullptr;

    void UpdateHeightOnAllFrames();

    //meshed as a 2D array, from outer to inner
    //circle is folded out to a surface, as if theres a cut seem
    //between 0 and array.num()
    TArray<FMeshedSurfaceFrame> edge;

    // mesh generation process
    bool AreaOfPolygonAroundCenterExceedsLimit(
        const TArray<FVector> &polygon,
        TArray<FVector> &outScaledDown,
        float scaleDownWidth // inward push in cm
    );

    float AreaOfPolygonAroundCenter(
        const TArray<FVector> &polygon,
        const FVector &center
    );


    // ----- NEW ------

    void RasterizePolygonEdgeTo(
        TArray<FVector> &polygon,
        float widthOfInsideStep
    );

    //debug
    void Draw(TArray<FVector> &shape);
    void Draw(FVector a, FVector b, float offset, FColor color);



    /// @brief extension to show grid as position and flag grid for free area.
    FMeshedSurfaceGrid meshedGrid;
};