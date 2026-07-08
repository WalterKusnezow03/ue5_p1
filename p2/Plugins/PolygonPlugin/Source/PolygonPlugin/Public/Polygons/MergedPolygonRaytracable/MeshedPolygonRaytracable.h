#pragma once

#include "CoreMinimal.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/MeshedPolygonHullIndices.h"

// ----- TODO! ------
//allows a position, a viewcone angle and ray density 
//to trace a visiblity cone on the seperate grid

class POLYGONPLUGIN_API FMeshedPolygonRaytracable : public FMeshedPolygon{

public:
    void TraceCone(const FVector &pos, const FVector2D &dir, float angle);
    void TraceConeOnGrid(
        const FVector &pos,
        const FVector2D &dir,
        float angle,
        // expects grid to be same size and space as this polygon (externally passable for multichannel cones)
        TArray<TArray<float>> &grid 
    );
    void TraceConeOnGrid(
        const FVector &pos, const FVector2D &dir, float angle, float rays,
        // expects grid to be same size and space as this polygon (externally passable for multichannel cones)
        TArray<TArray<float>> &grid 
    );
    void TraceConeOnGridBetweenDirections(
        const FVector &pos,
        const FVector2D &limitA,
        const FVector2D &limitB,
        TArray<TArray<float>> &grid // expects grid to be same size and space as this polygon
    );

    virtual void AppendAsBinary(
        TArray<uint8> &buffer
    ) override;

    virtual bool LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *&Ptr // reference to a pointer. Pointer by reference.
    ) override;

    //perform visiblity check to verify NN prediciton
    bool IsVisible(const FVector &a, const FVector &b);

    virtual void ResizeGrid(int x, int y) override;

protected:
    void RecreateBoundHull();

    void TraceConeOnGrid(
        int x, int y, const FVector2D &dir, float angle, float rays,
        // expects grid to be same size and space as this polygon (externally passable for multichannel cones)
        TArray<TArray<float>> &grid,
        bool dirIsCentered
    );

    void TraceConeCollectHits(
        int x, int y,
        const FVector2D &dir,
        float angle,
        float rays,
        TArray<FIntPoint> &hits,
        bool dirIsCentered
    );

    void CreateOrClearViewGrid();
    TArray<TArray<float>> viewGrid;
    bool ViewGridValid();

    FMeshedPolygonHullIndices boundHull;

    bool Trace(int x, int y, const FVector2D &dir, FIntPoint &outHit);
    bool Trace(int x, int y, const FVector2D &dir, FIntPoint &outHit, bool ignoreBounds);
    bool Trace(int x, int y, const FVector2D &dir, FIntPoint &outHit, bool ignoreBounds, float &outT);
    void FlagPositon(const FIntPoint &pos, TArray<TArray<float>> &grid);

    void DrawLineFromPositionToHits(
        FIntPoint &start,
        TArray<FIntPoint> &hits,
        TArray<TArray<float>> &grid
    );

    void FlagPositionsFromPolygon(TArray<FIntPoint> &positionsRaw, TArray<TArray<float>> &grid);

    void BresenhamLineAppend(
        const FIntPoint &Start, 
        const FIntPoint &End,
        TArray<FIntPoint> &outPoints
    );

    void InjectStartingPointAtFrontAndEnd(
        TArray<FIntPoint> &hits, // not rasterized properly yet
        const FIntPoint &start
    );

    FString MakeString(const TArray<FIntPoint> &hits);

    FString ViewGridAsString();

    void ApplyGaussViewGrid();
    void ApplyGaussViewGrid(int sizeMask, float sigma);

private:
    float viewGridClearedValue = 0.0f;
    float viewGridTrueValue = 1.0f;

    FVector2D MakeDir(const FVector &v0, const FVector &v1);
};