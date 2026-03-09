#pragma once

#include "PlueckerCore/Math/SpatialTransforms/SpatialTransform.h"

class PLUECKERCORE_API SpatialTransformRoot : public SpatialTransform {

public:
    //local transform of root with: M = T * R <-- lese richtung --
    MMatrix Transform() override;

    //local transform inverse of root with: M = R^-1 * T^-1 <-- lese richtung --
    MMatrix TransformInverse() override;

    //deprecated
    virtual FVector Torque(const FVector &force, const FVector &centerOfMass) override {
        return FVector(0, 0, 0);
    }
    virtual void applyJointConstraint(FVector &w, FVector &v) override;

protected:
    FVector none = FVector(0, 0, 0);
    virtual FVector &axis() override; //always none, we dont want torque across the root translation from origin
    //would create very fucking weird results.

    //saves new world translation with integrated plücker result
    virtual void OnForwardPlueckerFinished() override;
    void OnForwardPlueckerFinishedCutTranslationAgainstGround();

    virtual void UpdateFloorContact(const MMatrix &prev, const MMatrix &current) override;
    virtual void SafeWorldResultCache(const MMatrix &prevTransform, const MMatrix &endBone) override;
};