#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

class COREMATH_API MPolygon {

public:
    MPolygon();
    ~MPolygon();

    ///@brief recommended to start at 0,0, rotated around this point.
    void SetShape(const TArray<FVector2D> &shapeIn);

    bool DoesIntersectClockwiseShape(TArray<FVector2D> &shapeIn);
    bool DoesIntersect(FVector2D &aWorld, FVector2D &bWorld);

    /// modify the translation and rotation to check again for hit
    void SetTranslation(const MMatrix2D &matrix);
    void SetRotation(const MMatrix2D &matrix);

    /// @brief recreates the shapeTransformed buffer for intersection tests.
    /// called automatically on transformation update and shape override (SetShape)
    void UpdateTransformedShape();

    void CopyShapeTransformed(TArray<FVector2D> &appendTo);

private:
    MMatrix2D myTranslation;
    MMatrix2D myRotation;

    TArray<FVector2D> shapeTransformed;
    TArray<FVector2D> shapeOriginal;

    bool DoesIntersect(
        FVector2D &aWorld, // edge 1
        FVector2D &bWorld,
        FVector2D &e1, // edge 2
        FVector2D &e2
    );

    float DotProduct(FVector2D &a, FVector2D &b);

    bool SegmentIntersection2D(
        FVector2D &a0,
        FVector2D &a1,
        FVector2D &b0,
        FVector2D &b1,
        FVector2D &outIntersection
    );

    FVector2D moveAwayFromIntersection;
};