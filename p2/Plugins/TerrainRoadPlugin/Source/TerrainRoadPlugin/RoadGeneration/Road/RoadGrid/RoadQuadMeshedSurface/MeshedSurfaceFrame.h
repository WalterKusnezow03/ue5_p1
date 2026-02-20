#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/MeshData/intersectCache/QuadIntersectFrame.h"

class TerrainInterfaceBase;
class MeshData;

//will store the quad and its neigbors, as well as a used flag
//will help with building area determination
class TERRAINROADPLUGIN_API FMeshedSurfaceFrame {

public:
    FMeshedSurfaceFrame();
    ~FMeshedSurfaceFrame();

    FMeshedSurfaceFrame(const FMeshedSurfaceFrame &other);
    FMeshedSurfaceFrame &operator=(const FMeshedSurfaceFrame &other);

    FMeshedSurfaceFrame(
        const FVector &v0In,
        const FVector &v1In,
        const FVector &v2In,
        const FVector &v3In
    );

    void Setup(
        const FVector &v0In,
        const FVector &v1In,
        const FVector &v2In,
        const FVector &v3In
    );

    void UpdateHeightOnVertecies(TerrainInterfaceBase *creator);

    void AppendMeshedSurface(MeshData &data);

protected:
    void UpdateHeight(TerrainInterfaceBase *creator, FVector &vertex, float offset);

    FVector v0;
    FVector v1;
    FVector v2;
    FVector v3;

};