#pragma once

#include "CoreMinimal.h"

class terrainCreator;
class MeshData;

/// grid from generated b spline
class TERRAINPLUGIN_API FMeshedSurfaceGrid {

public:
    FMeshedSurfaceGrid();
    ~FMeshedSurfaceGrid();

    void Init(TArray<FVector> &polygon, float widthOfInsideStep);
    void UpdateHeights(terrainCreator *creator);

    //debug
    void AppendMeshedSurface(MeshData &data);
private:
    
    void FindBounds(TArray<FVector> &polygon);
    void GetSizeGrid(int &x, int &y);
    void GetSizeGrid(int &x, int &y, float widthOfInsideStep);

    TArray<TArray<bool>> flagGrid;
    TArray<TArray<FVector>> positionGrid;

    FVector minSaved;
    FVector maxSaved;
    float stepSizeSaved;

    void GenerateGrid();
    bool GridValid();

    void FlagTrue(const TArray<FVector> &polygon);
    void FlagTrue(const FVector &pos);
    bool ToIndex(const FVector &pos, int &x, int &y);

    bool FlagAt(int x, int y);
    bool PositionAtFlag(int x, int y, FVector &outPos);

    void FlagBetweenSpaceTrue();
    void FlagBetweenSpaceTrue(TArray<bool> &flagBuffer);
    void FlagBetweenSpaceTrue(TArray<bool> &flagBuffer, int i, int j);

    
    void UpdateHeight(terrainCreator *creator, FVector &vertex, float offset);


    //debug
    void AppendAt(int i, int j, MeshData &data);
    TArray<FVector> GetQuadOrTriangleAt(int i, int j);
};