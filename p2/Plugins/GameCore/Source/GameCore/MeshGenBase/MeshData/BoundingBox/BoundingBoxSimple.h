#pragma once


#include "CoreMinimal.h"
#include "PhysicsEngine/BoxElem.h"

/**
 * Boudning box checker withoutout rotation, only min and max (x,y,z)
 */
class GAMECORE_API BoundingBoxSimple{

public:
    BoundingBoxSimple();
    BoundingBoxSimple(TArray<FVector> &vertecies);
    BoundingBoxSimple(TArray<FVector3f> &vertecies);
    ~BoundingBoxSimple();

    BoundingBoxSimple &operator=(const BoundingBoxSimple &other);
    BoundingBoxSimple(const BoundingBoxSimple &other);

    FVector bottomLeftNearVertex();
    FVector topRightLeftVertex();


    void updateBoundsIfNeeded(TArray<FVector> &vertecies);
    void updateBoundsIfNeeded(FVector &other);

    void RebuildBounds(TArray<FVector> &vertecies);

    bool isInsideBoundingbox(FVector &other);

    FString ToString();

    bool DoesIntersect(const FVector &origin, const FVector &directionScaled);

    static void Test();

    // api for engine related collision creation
    void Update(FKBoxElem *elem) const;
    FVector extent() const;

private:
    FVector bottomLeftNear;
    FVector topRightLeft;

    //for FKBoxElem
    float WidthY() const;
    float HeightZ() const;
    float DepthX() const;
    FVector center() const;

    bool InRange(float t, float lower, float higher);
};