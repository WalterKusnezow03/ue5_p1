#pragma once

#include "CoreMinimal.h"


struct CUSTOMUIPLUGIN_API FBoundingBox2D {

    FBoundingBox2D();
    ~FBoundingBox2D();
    FBoundingBox2D(const FBoundingBox2D & other);
    FBoundingBox2D &operator=(const FBoundingBox2D & other);

    FBoundingBox2D(TArray<FVector2D> &buffer);

    void Update(TArray<FVector2D> &buffer);

    void Update(FVector2D &pos);

    void Update(FVector2D &v0, FVector2D &v1, FVector2D &v2);

    FVector2D bottomLeft;
    FVector2D topRight;

    float sizeX();
    float sizeY();
};