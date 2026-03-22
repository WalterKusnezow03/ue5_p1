// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

/**
 * Bounding shape which tests with right left test if a point is inside.
 * Reqquires a sorted shape to be added!
 */
class MESHDATAPLUGIN_API BoundingShapeXY {

public:
    BoundingShapeXY();
    ~BoundingShapeXY();

    void initWithSortedQuad(FVector &a, FVector &b, FVector &c, FVector &d);
    void initWithSortedShape(TArray<FVector> &ref);
    bool isInsideShape(FVector &test);


private:
    bool rightOff(FVector &a, FVector &b, FVector &target);
    TArray<FVector> shape;
};