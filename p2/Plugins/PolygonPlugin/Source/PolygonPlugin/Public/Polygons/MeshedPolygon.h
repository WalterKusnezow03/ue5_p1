#pragma once

#include "CoreMinimal.h"


class MeshData;

/// grid from generated b spline
///data is transfered to position grid and boolean grid for location being
///present inside the polygon
class POLYGONPLUGIN_API FMeshedPolygon {

protected:
    TArray<TArray<bool>> flagGrid;
    TArray<TArray<FVector>> positionGrid;
    TArray<std::pair<int, int>> edgeIndices;

    FVector minSaved;
    FVector maxSaved;
    float stepSizeSaved;


public:
    FMeshedPolygon();
    virtual ~FMeshedPolygon();

    FMeshedPolygon(const FMeshedPolygon &other);
    FMeshedPolygon &operator=(const FMeshedPolygon &other);

    //init from rasterized curve
    void Init(TArray<FVector> &polygon, float widthOfInsideStep);
    

    //debug
    void AppendMeshedSurface(MeshData &data);

    bool IsValid();
    FVector BottomLeft();

protected:


    
    void FindBounds(TArray<FVector> &polygon);
    void GetSizeGrid(int &x, int &y);
    void GetSizeGrid(int &x, int &y, float widthOfInsideStep);

    

    void GenerateGrid();
    bool GridValid();

    void FlagTrue(const TArray<FVector> &polygon);

    void FlagTrueInterpolate(
        const FVector &v0,
        const FVector &v1
    );

    //for interpolation
    void GenerateStepDirectionForInterpolation(
        const FVector &v0,
        const FVector &v1,
        int &outSteps,
        FVector &outDirStepSized
    );

    void FlagTrue(const FVector &pos);
    void FlagTruePolygonEdge(const FVector &pos);
    void ToIndexBounded(const FVector &pos, int &x, int &y);
    bool FlagAtPosition(const FVector &pos);

    bool FlagAt(int x, int y);
    bool PositionAtFlag(int x, int y, FVector &outPos);

    //override flags by batch
    void SetFlag(const TArray<std::pair<int, int>> &indexPositions, bool flag);
    void SetFlag(const std::pair<int, int> &indexPos, bool flag);


    bool PositionAt(int x, int y, FVector &outPos);
    bool PositionAt(const std::pair<int, int> &pair, FVector &outPos);
    FVector GetPositionAt(const std::pair<int, int> &pair);
    FVector Rotation(
        const std::pair<int, int> &posAPair,
        const std::pair<int, int> &posBPair
    );

    void FlagBetweenSpaceTrue();
    void FlagBetweenSpaceTrue(TArray<bool> &flagBuffer);
    void FlagBetweenSpaceTrue(TArray<bool> &flagBuffer, int i, int j);


    //debug
    void AppendAt(int i, int j, MeshData &data);
    TArray<FVector> GetQuadOrTriangleAt(int i, int j);


};