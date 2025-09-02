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

    float DistanceSquared(const FVector2D &other) const {
        return FVector2D::DistSquared(other, position);
    }
};