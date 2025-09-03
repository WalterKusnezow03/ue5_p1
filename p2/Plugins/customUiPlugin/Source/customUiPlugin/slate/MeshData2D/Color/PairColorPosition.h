#pragma once

#include "CoreMinimal.h"

struct CUSTOMUIPLUGIN_API FPairColorPosition {

    FPairColorPosition() {};
    ~FPairColorPosition() {};

    FPairColorPosition(
        FLinearColor &colorIn,
        FVector2D &posIn
    ) {
        color = colorIn;
        position = posIn;
    };

    FPairColorPosition(
        FLinearColor &colorIn,
        FVector2D &posIn,
        FVector2D &uvIn
    ) : FPairColorPosition(colorIn, posIn)
    {
        uv = uvIn;
    };

    FPairColorPosition(const FPairColorPosition &other){
        if(this != &other){
            *this = other;
        }
    }
    FPairColorPosition &operator=(const FPairColorPosition &other){
        if(this != &other){
            color = other.color;
            position = other.position;
            uv = other.uv;
        }
        return *this;
    }

    ///@brief updates the position and returns if it changed
    bool UpdatePosition(FVector2D &pos){
        bool update = DistanceSquared(pos) >= 1.0f;
        if(update){
            position = pos;
        }
        return update;
    }

    ///@brief updates the color and returns if it changed
    bool UpdateColor(FLinearColor &colorIn){
        bool update = color != colorIn;
        color = colorIn;
        return update;
    }

    FLinearColor color;
    FVector2D position;
    FVector2D uv;

    float DistanceSquared(const FVector2D &other) const {
        return FVector2D::DistSquared(other, position);
    }
};