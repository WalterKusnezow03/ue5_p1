#pragma once

#include "CoreMinimal.h"

class SlateMeshData;
class SlateMeshDataPolygon;


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

    void Reset();

    // basic update
    FBoundingBox2D(const TArray<FVector2D> &buffer);
    void Update(const TArray<FVector2D> &buffer);
    void Update(const FVector2D &pos);
    void Update(const FVector2D &v0, const FVector2D &v1, const FVector2D &v2);

    //update from polygon and meshdata
    void Update(const SlateMeshData &ref);
    void Update(const SlateMeshDataPolygon &ref);

    FVector2D topLeft;
    FVector2D bottomRight;

    FVector2D size() const;
    float sizeX() const;
    float sizeY() const;
};