#pragma once

#include "CoreMinimal.h"
#include "PolygonPlugin/Public/GridBase/GridBase.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/MeshedPolygonHullIndices.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/MeshedPolygonHullSet.h"

class MeshData;

/// grid from generated b spline
///data is transfered to position grid and boolean grid for location being
///present inside the polygon
// true: blocked, false: free
class POLYGONPLUGIN_API FMeshedPolygon : public GridBase{

private:
    int MINSTEP = 10.0f;

protected:
    TArray<TArray<uint8>> flagGrid;
    
    //edge indices of polygon
    //TArray<std::pair<int, int>> edgeIndices;

    //edge indices of polygon
    //FMeshedPolygonHullIndices edgeIndices;

    FMeshedPolygonHullSet edgeSet;

    FMeshedPolygonHullIndices &GetEdgeIndices(){
        //return at 0
        return edgeSet[0];
    }

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



    virtual FVector BottomLeft(){
        return minSaved;
    }

    virtual FVector TopRight(){
        return maxSaved;
    }

    int NumEdges();

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
    

    //joined bit map from poylgons
    void GenerateFrom(
        const std::vector<FMeshedPolygon *> &polygons,
        const FVector &capA, 
        const FVector &capB,
        float stepSizeIn
    );

    int sizeX();
    int sizeY();


    // --- index hull ---
    void AppendIndexHull(const TArray<FVector> &polygon);
    void AppendIndexHull(const TArray<FVector> &polygon, FMeshedPolygonHullIndices &hull);

    // --- storage interface ---

    TArray<TArray<uint8>> &GetFlagGrid();
   
    float GetStepSizeSaved() const;
    void GetMinMax(FVector &minOut, FVector &maxOut);

    void OverrideStepSize(float sizeIn);

    void OverrideMinMax(FVector &minIn, FVector &maxIn);


    const TArray<TArray<uint8>> &GetFlagGridConst() const;
    

    // --- storage interface ---

    //swaps 0 and 1 flags
    void InvertFlagMap();

protected:
    void RemoveMapBorder(TArray<TArray<float>> &grid, int sizeBorder, float value);
    void RemoveBorder(TArray<float> &column, int sizeBorder, float value);

    void InvertFlag(uint8 &flagCurrent);
    bool flagsInverted = false; //track inversion state


    //forces a min size of one if bounds to small for pixel step "widthOfInsideStep"
    virtual bool InitAsSinglePixel(TArray<FVector> &polygon, float widthOfInsideStep);
    FVector center(TArray<FVector> &polygon);

    void FindBounds(TArray<FVector> &polygon);
    void FindBounds(FVector bottomLeft, FVector topRight, bool safeCheck);
    void GetSizeGrid(int &x, int &y);
    void GetSizeGrid(int &x, int &y, float widthOfInsideStep);

    bool IsInBound(const FVector &pos);
    bool IsInBound(const FVector &pos, int &xOut, int &yOut);

    bool FlagIndexInBound(const int x, const int y);

    virtual void GenerateGrid();
    virtual bool GridValid() const;

    void FlagTrue(const TArray<FVector> &polygon);

    void FlagTrueInterpolate(
        const FVector &v0,
        const FVector &v1
    );

    //for interpolation
    bool GenerateStepDirectionForInterpolation(
        const FVector &v0,
        const FVector &v1,
        int &outSteps,
        FVector &outDirStepSized
    );

    void FlagTrue(const FVector &pos);
    void FlagTruePolygonEdge(const FVector &pos);
    void ToIndexBounded(const FVector &pos, int &x, int &y);
    void ToIndexRaw(const FVector &pos, int &x, int &y);
    void ToIndexRaw(const FMeshedPolygon &other, int &x, int &y);
    bool FlagAtPosition(const FVector &pos);

    bool FlagAt(int x, int y);
    //bool PositionAtFlag(int x, int y, FVector &outPos);

    //override flags by batch
    void SetFlag(const TArray<std::pair<int, int>> &indexPositions, bool flag);
    void SetFlag(const std::pair<int, int> &indexPos, bool flag);
    void SetFlag(int x, int y, bool flag);
    void SetFlagInt(int x, int y, uint8 flag);
    void SetFlag(const FVector &pos, bool flagIn);

    //void SetPosition(int x, int y, const FVector &pos);

    //bool PositionAt(int x, int y, FVector &outPos);
    //bool PositionAt(const std::pair<int, int> &pair, FVector &outPos);
    //FVector GetPositionAt(const std::pair<int, int> &pair);
    FVector Rotation(
        const std::pair<int, int> &posAPair,
        const std::pair<int, int> &posBPair
    );

    void FlagBetweenSpaceTrue();
    void FlagBetweenSpaceTrue(TArray<uint8> &flagBuffer);
    void FlagBetweenSpaceTrue(TArray<uint8> &flagBuffer, int i, int j);

    
    //debug
    //void AppendAt(int i, int j, MeshData &data);
    //TArray<FVector> GetQuadOrTriangleAt(int i, int j);

    void FlagTrueFromFastAndAppendLocalHulls(
        const FMeshedPolygon &other
    );
    void AppendLocalHulls(
        const int xStart,
        const int yStart,
        const FMeshedPolygon &other
    );

    void FlagTrueFromFast(
        const int xStart,
        const int yStart,
        const FMeshedPolygon &other
    );
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

    
    //void MakePositionGrid();

    void GetResolution(int &xOut, int &yOut) const;

    uint8 FlagAsInt8(bool flag);

public:
    void ClearFlags();
    void ClearFlagsAndEdgeSet();
    void ClearEdgeSet();

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



protected:
    //template grid fill
    template <typename T>
    void TClearGrid(TArray<TArray<T>> &grid, T valueNone){
        for (int i = 0; i < grid.Num(); i++){
            TArray<T> &col = grid[i];
            for (int j = 0; j < col.Num(); j++){
                col[j] = valueNone;
            }
        }
    }

    template <typename T>
    void TOverrideValue(TArray<TArray<T>> &grid, int i, int j, T value){
        if(i >= 0 && j >= 0 && i < grid.Num() && j < grid[i].Num()){
            grid[i][j] = value;
        }
    }

    //creates grid with same size as flag grid
    template <typename T>
    void TCreateOrClearGrid(TArray<TArray<T>> &grid, T defaultNoneValue){
        //grid size from bool grid
        int x = sizeX();
        int y = sizeY();

        //get size from bounds if failed.
        if(x <= 0 && y <= 0){
            GetSizeGrid(x, y);
        }


        if(x > 0 && y > 0){
            if (TGridIsSize<T>(x, y, grid)){
                TClearGrid<T>(grid, defaultNoneValue);
                return;
            }
            TGenerateGrid<T>(x, y, grid);
            TClearGrid<T>(grid, defaultNoneValue);
        }
    }

    FVector PositionFromIndex(int i, int j);

public:
    virtual void AppendAsBinary(
        TArray<uint8> &buffer
    );
    virtual bool LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *& Ptr
    );



    // ---- resize ----
    virtual void ResizeGrid(int x, int y);

protected:
    template <typename T>
    void TResizeGrid(
        TArray<TArray<T>> &originalGrid, 
        T defaultNoneValue,
        int xWanted,
        int yWanted
    ){
        if(xWanted <= 0 || yWanted <= 0){
            return;
        }
        if(TGridIsSize<T>(xWanted, yWanted, originalGrid)){
            return;
        }

        TArray<TArray<T>> gridSmaller; //or larger
        TGenerateGrid<T>(xWanted, yWanted, gridSmaller);
        TClearGrid<T>(gridSmaller, defaultNoneValue);

        for (int i = 0; i < gridSmaller.Num(); i++){
            TArray<T> &column = gridSmaller[i];
            if(i < originalGrid.Num()){
                TArray<T> &other = originalGrid[i];

                //void* FMemory::Memcpy(void* Dest, const void* Src, SIZE_T Count);
                int32 smallerSize = FMath::Min(column.Num(), other.Num());
                if(smallerSize > 0){
                    FMemory::Memcpy(column.GetData(), other.GetData(), sizeof(T) * smallerSize);
                }
            }
        }
        originalGrid = gridSmaller; //override
    }
};