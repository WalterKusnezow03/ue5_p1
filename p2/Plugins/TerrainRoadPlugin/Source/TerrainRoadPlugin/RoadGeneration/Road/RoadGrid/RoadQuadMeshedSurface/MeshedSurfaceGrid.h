#pragma once

#include "CoreMinimal.h"

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

class TerrainInterfaceBase;
class MeshData;

/// grid from generated b spline
///data is transfered to position grid and boolean grid for location being
///present inside the polygon
class TERRAINROADPLUGIN_API FMeshedSurfaceGrid : public FMeshedPolygon{

public:
    FMeshedSurfaceGrid();
    ~FMeshedSurfaceGrid();

    FMeshedSurfaceGrid(const FMeshedSurfaceGrid &other);
    FMeshedSurfaceGrid &operator=(const FMeshedSurfaceGrid &other);

    
    void UpdateHeights(TerrainInterfaceBase *creator);

    //no rotation

    bool FindShape(
        int x, // in cm
        int y, // in cm
        FVector &outBottomLeft,
        FVector &outRotation
    );

private:
    bool DistanceEnough(
        const std::pair<int, int> &posA,
        const std::pair<int, int> &posB,
        int desired
    );
    int Distance(
        const std::pair<int, int> &posA,
        const std::pair<int, int> &posB
    );

    void UpdateHeight(TerrainInterfaceBase *creator, FVector &vertex, float offset);


    //if true returned: success
    bool LockArea(
        const std::pair<int, int> &posA,
        const std::pair<int, int> &posB,
        FVector &rotation,
        int depth // in cm
    );

    FVector Rotation(
        const std::pair<int, int> &posAPair,
        const std::pair<int, int> &posBPair
    );
};