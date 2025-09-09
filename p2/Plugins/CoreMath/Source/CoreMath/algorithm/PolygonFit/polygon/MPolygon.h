#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

class COREMATH_API MPolygon {

public:
    MPolygon();
    ~MPolygon();

    MPolygon(const MPolygon &other);
    MPolygon &operator=(const MPolygon &other);

    ///@brief recommended to start at 0,0, rotated around this point.
    void SetShape(const TArray<FVector2D> &shapeIn);

    bool DoesIntersect(const MPolygon &shapeIn);
    bool DoesIntersectClockwiseShape(const TArray<FVector2D> &shapeIn);
    bool DoesIntersect(const FVector2D &aWorld, const FVector2D &bWorld);

    /// modify the translation and rotation to check again for hit
    void SetTranslation(const FVector2D &matrix);
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

    FVector2D moveAwayFromIntersection;

    bool DoesIntersect(
        const FVector2D &aWorld, // edge 1
        const FVector2D &bWorld,
        const FVector2D &e1, // edge 2
        const FVector2D &e2
    );

    float DotProduct(const FVector2D &a, const FVector2D &b);

    bool SegmentIntersection2D(
        const FVector2D &a0,
        const FVector2D &a1,
        const FVector2D &b0,
        const FVector2D &b1,
        FVector2D &outIntersection
    );

    
};