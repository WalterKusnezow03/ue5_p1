#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadGrid/RoadQuadMeshedSurface/MeshedSurfaceFrame.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadGrid/RoadQuadMeshedSurface/MeshedSurfaceGrid.h"

class RoadQuad;
class terrainCreator;
class MeshData;

//will store a set of quads from the road quad and terrain creator heights
//inidididual quads will be assigned for buildings
//search functions for a convex area will be searchable
class TERRAINPLUGIN_API RoadQuadMeshedSurface {

public:
    RoadQuadMeshedSurface();
    ~RoadQuadMeshedSurface();

    RoadQuadMeshedSurface(const RoadQuadMeshedSurface &other);
    RoadQuadMeshedSurface &operator=(const RoadQuadMeshedSurface &other);

    void Init(
        RoadQuad &quad, 
        terrainCreator *creator,
        float widthOfInsideStep
    );

    static void ScaleDownByWidth(
        const TArray<FVector> &circle,
        TArray<FVector> &OutInnerCircle,
        float width
    );
    static void ScaleDownByWidth(
        const TArray<FVector> &circle,
        TArray<FVector> &OutInnerCircle,
        float width,
        const FVector &center
    );

    //debug
    void AppendMeshedSurface(MeshData &data);

private:
    void Init(
        RoadQuad &quad,
        float widthOfInsideStep
    );

    void SetTemporaryTerrainCreatorReference(terrainCreator *creator);
    void ResetTemporaryTerrainCreatorReference();

    terrainCreator *temporaryReference = nullptr;

    void UpdateHeightOnAllFrames();

    //meshed as a 2D array, from outer to inner
    //circle is folded out to a surface, as if theres a cut seem
    //between 0 and array.num()
    TArray<TArray<FMeshedSurfaceFrame>> surface;

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
    void RasterizeVerteciesTo(
        TArray<FVector> &polygon,
        float widthOfInsideStep
    );
    FVector ToModCoordinate(FVector &pos, int mod, int dir);

    FVector& ChooseRightOffCoordinateRelativeTo(
        FVector &modLower,
        FVector &modHigher,
        FVector &normal,
        FVector &relativePoint
    );

    //debug
    void Draw(TArray<FVector> &shape);
    void Draw(FVector a, FVector b, float offset, FColor color);

    // ----- NEW EXTENSION -----
    FMeshedSurfaceGrid meshedGrid;
};