#pragma once

#include "CoreMinimal.h"

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonExtension/MeshedPolygonPositionField.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/MeshedPolygonHullSet.h"

class TerrainInterfaceBase;
class MeshData;

/// grid from generated b spline
///data is transfered to position grid and boolean grid for location being
///present inside the polygon
class TERRAINROADPLUGIN_API FMeshedSurfaceGrid : public FMeshedPolygonPositionField {//FMeshedPolygon{

public:
    FMeshedSurfaceGrid();
    ~FMeshedSurfaceGrid();

    FMeshedSurfaceGrid(const FMeshedSurfaceGrid &other);
    FMeshedSurfaceGrid &operator=(const FMeshedSurfaceGrid &other);

    
    void UpdateHeights(TerrainInterfaceBase *creator);

    //prepare before find hull - on raw hull
    void SplitSelfCuttingPolygonsFromOuterHull();

    // on raw hull
    bool CanFindShape();
    bool FindShape(
        int x, // in cm
        int y, // in cm
        FVector &outBottomLeft,
        FRotator &outRotation
    );

    

    //debug - in world space / grid data world
    void DebugAppendEdgeSetRawAsMeshData(float height, MeshData &other, float offset);
    void DebugAppendEdgeSetRawAsMeshData(float height, MeshData &other, const FVector offset);
    void DebugAppendEdgeSetRawAsMeshData(
        float height,
        MeshData &other,
        FMeshedPolygonHullIndices &hull,
        const FVector offset
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
        FVector &pivot,
        const FVector2D &stepDir,
        const FVector2D &inset,
        int xIndicesNeeded,
        int yIndicesNeeded,
        FRotator &outRotation
    );

    FVector Rotation(
        const std::pair<int, int> &posAPair,
        const std::pair<int, int> &posBPair
    );

    void Draw(
        const FVector2D &v0,
        const FVector2D &v1,
        const FVector2D &v2,
        const FVector2D &v3,
        float offset,
        FColor color
    );
    void Draw(
        const TArray<FVector2D> &array,
        float offset,
        FColor color
    );

    bool PolygonCanBeAdded(
        const TArray<FVector2D> &array,
        bool &insidePolygon,
        bool &visible 
    );
    

    bool BoundsSmallerThanStepSize(int x, int y);

    void MakeBounds(
        int x,
        int y,
        TArray<FVector2D> &outArray,
        FVector &pivot,
        const FVector2D &rotation,
        const FVector2D &inset,
        FRotator &outRotation
    );
    
    void MakeBounds(int x, int y, TArray<FVector2D> &outArray);
    FRotator MakeRotator(const FVector2D &rotation);

    FVector2D MakeInsetFromEdgeAngle(float dot, float depth);

    bool IsCorner(float dot);
    bool IsCorner(float dot, float epsilon);

private:
    float cornerUntilDot = 0.2f;

    void DrawMarkers(TArray<FVector> &positions, FColor color);
    void DrawMarker(FVector pos, FColor color);
    void DrawConnect(FVector posA, FVector posB, float offset, FColor color);
};