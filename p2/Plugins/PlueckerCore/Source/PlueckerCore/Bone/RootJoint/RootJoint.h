#pragma once

#include "CoreMinimal.h"
#include "PlueckerCore/Math/SpatialTransforms/SpatialTransformRoot.h"

/// @brief expicit 6DOF joint but T * R translation instead of R * T
class PLUECKERCORE_API RootJoint : public Joint {

protected:
    SpatialTransform &GetSpatialTransform() override {
        return rootTransform;
    }

    virtual const SpatialTransform &GetSpatialTransformConst() const override {
        return rootTransform;
    }

private:
    SpatialTransformRoot rootTransform;

    void RemoveConstraints();

public:
    RootJoint();
    RootJoint(FVector location, UWorld *world);
    RootJoint(const RootJoint &other);
    RootJoint &operator=(const RootJoint &other);
    virtual ~RootJoint();

    virtual void SetInteriaMatrixAuto() override;

    void TickAndBuildRecursive(float deltatime);

    
    virtual void OverrideWorldLocation(FVector pos) override;
    void OverrideJointWorldTransform(FVector pos, FRotator roation) override;

    void OverrideLinearVelocity(FVector &vIn);





};