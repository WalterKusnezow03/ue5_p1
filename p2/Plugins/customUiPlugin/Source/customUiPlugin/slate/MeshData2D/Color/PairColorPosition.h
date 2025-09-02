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

    FLinearColor color;
    FVector2D position;

    float DistanceSquared(const FVector2D &other) const {
        return FVector2D::DistSquared(other, position);
    }
};