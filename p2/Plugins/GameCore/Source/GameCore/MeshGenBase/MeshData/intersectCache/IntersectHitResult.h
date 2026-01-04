#pragma once

#include "CoreMinimal.h"


class GAMECORE_API FIntersectHitResult {

public:
    FIntersectHitResult(){}
    ~FIntersectHitResult(){}

    FVector hitPoint;
    FVector2D hitUV;

protected:


};