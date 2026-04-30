#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "MeshedPolygonPositionField.h"
#include "PolygonPlugin/Public/GridBase/GridBase.h"

class POLYGONPLUGIN_API FMeshedPolygonDistanceField : public FMeshedPolygonPositionField{

public:
    //void InitFrom(FMeshedPolygon &polygon);
    void InitDistanceField();

    virtual void GenerateColorBitmap(
        TArray<FColor> &outBuffer,
        FColor &free,
        FColor &blocked,
        int &resXOut,
        int &resYOut
    ) const override;

    

    FString ToString();

protected:
    
    TArray<TArray<float>> distanceField;
    bool IsValidDistanceField() const;

    void InitGridMax();

    void GenerateDistanceField(
        TArray<TArray<uint8>> &flagGrid,
        TArray<TArray<FVector>> &positionGrid
    );

    void GenerateDistanceFieldColumnIncrement(
        TArray<uint8> &flagColumn,
        TArray<FVector> &positionColumn,
        TArray<float> &outColumn
    );
    void GenerateDistanceFieldColumnDecrement(
        TArray<uint8> &flagColumn,
        TArray<FVector> &positionColumn,
        TArray<float> &outColumn
    );

    void GenerateDistanceFieldRowIncrement(
        TArray<TArray<uint8>> &flagGrid,
        TArray<TArray<FVector>> &positionGrid,
        TArray<TArray<float>> &outGrid,
        int row
    );

    void GenerateDistanceFieldRowDecrement(
        TArray<TArray<uint8>> &flagGrid,
        TArray<TArray<FVector>> &positionGrid,
        TArray<TArray<float>> &outGrid,
        int row
    );

    template <typename T>
    bool InBound(const TArray<T> &buffer, int index){
        return index >= 0 && index < buffer.Num();
    }

    FColor ColorDirection(const FColor &a, const FColor &b) const;
    FColor ScaledColor(const FColor &start, const FColor &dir, float scalar) const;
    void Validate(FColor &color) const;

    float MaxDistance()const;
    float MaxDistance(const TArray<float> &buffer)const;

    //void color generation
    void GenerateColorBitmap(
        TArray<FColor> &outBuffer,
        FColor &freeColor,
        FColor &blockedEnd
    )const;
    void GenerateColorBitmap(
        TArray<FColor> &outBuffer,
        FColor &freeColor,
        FColor &dir,
        const TArray<float> &distanceFieldColumn,
        float maxDistance
    )const;
};