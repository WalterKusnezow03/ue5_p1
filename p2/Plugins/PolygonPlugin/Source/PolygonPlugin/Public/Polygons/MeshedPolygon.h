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



public:
    FMeshedPolygon();
    virtual ~FMeshedPolygon();

    FMeshedPolygon(const FMeshedPolygon &other);
    FMeshedPolygon &operator=(const FMeshedPolygon &other);

    //init from rasterized curve, to raster of mod "widthOfInsideStep"
    void Init(TArray<FVector> &polygon, float widthOfInsideStep);

    //init from rasterized curve, to raster of mod "widthOfInsideStep"
    //forces a min size of one if bounds to small for pixel step "widthOfInsideStep"
    void InitForceSizeMin(TArray<FVector> &polygon, float widthOfInsideStep);

    //debug
    void AppendMeshedSurface(MeshData &data);

    bool IsValid();
    FVector BottomLeft();
    FVector TopRight();


    //joined bit map from poylgons
    void GenerateFrom(
        const std::vector<FMeshedPolygon *> &polygons,
        const FVector &capA, 
        const FVector &capB,
        float stepSizeIn
    );

    int sizeX();
    int sizeY();

    // --- storage interface ---

    TArray<TArray<bool>> &GetFlagGrid();
    TArray<TArray<FVector>> &GetPositionGrid();
    float GetStepSizeSaved();
    void GetMinMax(FVector &minOut, FVector &maxOut);

    void OverrideStepSize(float sizeIn);

    void OverrideMinMax(FVector &minIn, FVector &maxIn);

    // --- storage interface ---

protected:
    //forces a min size of one if bounds to small for pixel step "widthOfInsideStep"
    bool InitAsSinglePixel(TArray<FVector> &polygon, float widthOfInsideStep);
    FVector center(TArray<FVector> &polygon);

    void FindBounds(TArray<FVector> &polygon);
    void FindBounds(FVector bottomLeft, FVector topRight, bool safeCheck);
    void GetSizeGrid(int &x, int &y);
    void GetSizeGrid(int &x, int &y, float widthOfInsideStep);

    bool IsInBound(const FVector &pos);
    bool IsInBound(const FVector &pos, int &xOut, int &yOut);
    
    void GenerateGrid();
    bool GridValid() const;

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
    void ToIndexRaw(const FVector &pos, int &x, int &y);
    bool FlagAtPosition(const FVector &pos);

    bool FlagAt(int x, int y);
    bool PositionAtFlag(int x, int y, FVector &outPos);

    //override flags by batch
    void SetFlag(const TArray<std::pair<int, int>> &indexPositions, bool flag);
    void SetFlag(const std::pair<int, int> &indexPos, bool flag);
    void SetFlag(int x, int y, bool flag);
    void SetFlag(const FVector &pos, bool flagIn);

    void SetPosition(int x, int y, const FVector &pos);

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


    void FlagTrueFrom(const FMeshedPolygon &other);
    void FlagTrueFromBuffer(
        const TArray<bool> &flagBuffer,
        const TArray<FVector> &positions
    );


};