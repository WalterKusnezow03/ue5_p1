#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

class MeshData;

class TERRAINBUILDINGPLUGIN_API BuildingBounds {

public:
    void Setup(int x, int y);
    void Setup(int x, int y, int z);

    void Setup(const BuildingBounds &other); //size only
    
    void SetHeight(int z);

    int getSizeX() const;
    int getSizeY() const;

    void AssignTransform(FVector &posWorld, FRotator &rotation);
    void SetPolygonEmbeddedPivot(const FVector &pos);

    //overrides the array data
    //sets num to 4 by default.
    void GetOverrideBoundingVerteciesWorld(TArray<FVector> &outArray);

    void AppendDebugCube(MeshData &data, FVector &offset);
    void AppendDebugCube(MeshData &data, FVector &offset, float debugScale);

    const FVector &GetPivot();

private:
    //from prefab / asset
    int sizeX = 1; //index space / uniscale index size
    int sizeY = 1;
    int sizeZ = 1;

    //applied later when embedded into the terrain
    FVector assignedBottomLeftWorld;
    FRotator assignedRoation;

    MMatrix transform;
    void UpdateTransform();

    

    void BoundVerteciesRaw(TArray<FVector> &outArray);

    void TransformAll(TArray<FVector> &outArray, MMatrix &M);
};