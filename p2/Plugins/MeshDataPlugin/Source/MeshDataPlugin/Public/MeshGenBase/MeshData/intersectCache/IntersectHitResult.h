#pragma once

#include "CoreMinimal.h"


class MESHDATAPLUGIN_API FIntersectHitResult {

public:
    FIntersectHitResult(){}
    ~FIntersectHitResult(){}

    FVector hitPoint;
    FVector2D hitUV;

protected:


};