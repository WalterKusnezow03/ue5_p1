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
    void TraceCone(const FVector &pos, const FVector2D &dir, float angle, float rays);
    void TraceCone(int x, int y, const FVector2D &dir, float angle, float rays);

    virtual void AppendAsBinary(
        TArray<uint8> &buffer
    ) override;

    virtual bool LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *&Ptr // reference to a pointer. Pointer by reference.
    ) override;

protected:
    void CreateOrClearViewGrid();
    TArray<TArray<float>> viewGrid;
    bool ViewGridValid();

    FMeshedPolygonHullIndices boundHull;

    bool Trace(int x, int y, const FVector2D &dir, FIntPoint &outHit);
    void FlagPositon(const FIntPoint &pos);


    void FlagPositionsFromPolygon(
        TArray<FIntPoint> &hits, // not rasterized properly yet
        int x,
        int y
    );

    void FlagPositionsFromPolygon(
        TArray<FIntPoint> &hits, // not rasterized properly yet
        const FIntPoint &start
    );

    void FlagPositionsFromPolygon(TArray<FIntPoint> &positionsRaw);

    void BresenhamLineAppend(
        const FIntPoint &Start, 
        const FIntPoint &End,
        TArray<FIntPoint> &outPoints
    );

    void FlagBetweenSpace(float value);
    void FlagBetweenSpace(TArray<float> &flagBuffer, float value);
    void FlagBetweenSpace(TArray<float> &flagBuffer, int i, int j, float value);

    FString MakeString(const TArray<FIntPoint> &hits);

    FString ViewGridAsString();

private:
    float viewGridClearedValue = 0.0f;
    float viewGridTrueValue = 1.0f;



    
};