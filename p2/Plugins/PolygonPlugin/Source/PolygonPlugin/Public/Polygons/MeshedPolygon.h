#pragma once

#include "CoreMinimal.h"
#include "PolygonPlugin/Public/GridBase/GridBase.h"


class MeshData;

/// grid from generated b spline
///data is transfered to position grid and boolean grid for location being
///present inside the polygon
// true: blocked, false: free
class POLYGONPLUGIN_API FMeshedPolygon : public GridBase{

protected:
    TArray<TArray<uint8>> flagGrid;
    TArray<TArray<FVector>> positionGrid;
    TArray<std::pair<int, int>> edgeIndices;

    FVector minSaved;
    FVector maxSaved;
    float stepSizeSaved;


    //not saved to storage, debug only!
    FString name;
    //not saved to storage, debug only!
public:

    FString GetName(){
        return name;
    }

    void SetName(FString nameIn){
        name = nameIn;
    }


public:
    

    //init from rasterized curve, to raster of mod "widthOfInsideStep"
    void Init(TArray<FVector> &polygon, float widthOfInsideStep);

    //init from rasterized curve, to raster of mod "widthOfInsideStep"
    //forces a min size of one if bounds to small for pixel step "widthOfInsideStep"
    void InitForceSizeMin(TArray<FVector> &polygon, float widthOfInsideStep);

    //debug
    void AppendMeshedSurface(MeshData &data);

    bool IsValid() const;
    bool FlagGridIsValid() const;
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

    TArray<TArray<uint8>> &GetFlagGrid();
    TArray<TArray<FVector>> &GetPositionGrid();
    float GetStepSizeSaved() const;
    void GetMinMax(FVector &minOut, FVector &maxOut);

    void OverrideStepSize(float sizeIn);

    void OverrideMinMax(FVector &minIn, FVector &maxIn);


    const TArray<TArray<uint8>> &GetFlagGridConst() const;
    const TArray<TArray<FVector>> &GetPositionGrid() const;

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

    bool FlagIndexInBound(const int x, const int y);

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
    void SetFlagInt(int x, int y, uint8 flag);
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
    void FlagBetweenSpaceTrue(TArray<uint8> &flagBuffer);
    void FlagBetweenSpaceTrue(TArray<uint8> &flagBuffer, int i, int j);

    
    //debug
    void AppendAt(int i, int j, MeshData &data);
    TArray<FVector> GetQuadOrTriangleAt(int i, int j);


    

    void FlagTrueFromFast(const FMeshedPolygon &other);
    void FlagTrueFromFast(
        const int xStart,
        const int yStart,
        const TArray<TArray<uint8>> &flagGridIn
    );
    void FlagTrueFromFast(
        const int xStart,
        const int yStart,
        const int xOffset,
        const TArray<uint8> &flagBuffer // column
    );

    void ClearFlags();
    void MakePositionGrid();

    void GetResolution(int &xOut, int &yOut) const;

    uint8 FlagAsInt8(bool flag);

public:
    virtual void GenerateColorBitmap(
        TArray<FColor> &outBuffer,
        FColor &free,
        FColor &blocked,
        int &resXOut,
        int &resYOut
    ) const ;



    //binary extraction
    void AppendFlagMap(
        TArray<uint8> &buffer
    ) const;



};