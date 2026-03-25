// Copyright Walter Kusnezow All Rights Reserved.
#pragma once


#include "CoreMinimal.h"
#include "PhysicsEngine/BoxElem.h"
#include "CoreMath/Matrix/MMatrix.h"

/**
 * Boudning box checker withoutout rotation, only min and max (x,y,z)
 */
class MESHDATAPLUGIN_API BoundingBoxSimple{

public:
    BoundingBoxSimple();
    BoundingBoxSimple(const TArray<FVector> &vertecies);
    BoundingBoxSimple(const TArray<FVector3f> &vertecies);
    ~BoundingBoxSimple();

    BoundingBoxSimple &operator=(const BoundingBoxSimple &other);
    BoundingBoxSimple(const BoundingBoxSimple &other);

    FVector bottomLeftNearVertex();
    FVector topRightFarVertex();


    void updateBoundsIfNeeded(const TArray<FVector> &vertecies);
    void updateBoundsIfNeeded(const FVector &other);

    void RebuildBounds(const TArray<FVector> &vertecies);

    bool isInsideBoundingbox(const FVector &other) const;

    FString ToString();

    bool DoesIntersect(const FVector &origin, const FVector &directionScaled);

    static void Test();

    // api for engine related collision creation
    void Update(FKBoxElem *elem) const;
    FVector extent() const;

    void Clear();

    void debugDrawBounds(const MMatrix &transform, UWorld *world, FColor color, float time);

private:
    FVector bottomLeftNear;
    FVector topRightFar;

    //for FKBoxElem
    float WidthY() const;
    float HeightZ() const;
    float DepthX() const;
    FVector center() const;

    bool InRange(float t, float lower, float higher);


        
    void GetAllCorners(
        FVector &A0,
        FVector &A1,
        FVector &A2,
        FVector &A3,
        FVector &B0,
        FVector &B1,
        FVector &B2,
        FVector &B3,
        const MMatrix &mat
    );
    void GetAllCorners(
        FVector &A0,
        FVector &A1,
        FVector &A2,
        FVector &A3,
        FVector &B0,
        FVector &B1,
        FVector &B2,
        FVector &B3
    );
};