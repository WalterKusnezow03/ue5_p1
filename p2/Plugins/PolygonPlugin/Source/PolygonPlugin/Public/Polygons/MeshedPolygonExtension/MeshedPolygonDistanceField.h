#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/GridBase/GridBase.h"

class POLYGONPLUGIN_API FMeshedPolygonDistanceField : public GridBase{

public:
    void Init(FMeshedPolygon &polygon);

protected:
    TArray<TArray<float>> distanceField;

    void InitGridMax();

    void GenerateDistanceField(
        TArray<TArray<bool>> &flagGrid,
        TArray<TArray<FVector>> &positionGrid
    );

    void GenerateDistanceFieldColumnIncrement(
        TArray<bool> &flagColumn,
        TArray<FVector> &positionColumn,
        TArray<float> &outColumn
    );
    void GenerateDistanceFieldColumnDecrement(
        TArray<bool> &flagColumn,
        TArray<FVector> &positionColumn,
        TArray<float> &outColumn
    );

    template <typename T>
    bool InBound(const TArray<T> &buffer, int index){
        return index >= 0 && index < buffer.Num();
    }
};