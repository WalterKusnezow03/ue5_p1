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
    float rootMass = 10 * 100;

    void RemoveConstraints();

public:
    RootJoint();
    RootJoint(FVector location, UWorld *world);
    RootJoint(const RootJoint &other);
    RootJoint &operator=(const RootJoint &other);
    virtual ~RootJoint();

    virtual void SetInteriaMatrixAuto() override;

    void TickAndBuildRecursive(float deltatime);

    void OverrideJointWorldTransform(FVector pos, FRotator roation);
    virtual void OverrideWorldLocation(MMatrix mat);
    virtual void OverrideWorldLocation(FVector pos);
    virtual FVector GetWorldLocation() const override;

    void OverrideLinearVelocity(FVector &vIn);


    //update com on backward build own force
protected:
    virtual void FindSelfInteriaAndGravitySpatialMoment(
        FVector &outN, // torque
        FVector &outF // force
    ) override;
    void UpdateCenterOfMassOnBackwardBuildForce();
    //virtual FVector CenterOfMassWorld() const override;

};