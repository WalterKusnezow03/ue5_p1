#pragma once

#include "CoreMinimal.h"

/// @brief bounding box for 2D Slate mesh data
/*
(tl)
0,0------ 1,0
|          |
0,1-------1,1(br)
*/
struct CUSTOMUIPLUGIN_API FBoundingBox2D {

    FBoundingBox2D();
    ~FBoundingBox2D();
    FBoundingBox2D(const FBoundingBox2D & other);
    FBoundingBox2D &operator=(const FBoundingBox2D & other);

    FBoundingBox2D(TArray<FVector2D> &buffer);

    void Update(TArray<FVector2D> &buffer);

    void Update(FVector2D &pos);

    void Update(FVector2D &v0, FVector2D &v1, FVector2D &v2);

    FVector2D topLeft;
    FVector2D bottomRight;

    float sizeX();
    float sizeY();
};