#pragma once

#include "PlueckerCore/Math/SpatialTransforms/SpatialTransform.h"

class PLUECKERCORE_API SpatialTransformBone : public SpatialTransform {

public:
    //local transform of bone with: M = R * T <--lese richtung--
    MMatrix Transform() override;
    // local transform inverse of bone with: M = T^-1 * R^-1 <--lese richtung--
    MMatrix TransformInverse() override;

protected:
    //screw axis always the bone itself!
    virtual FVector &axis() override {
        return translation;
    }

    virtual void UpdateFloorContact(const MMatrix &prev, const MMatrix &current) override;
    virtual void SafeWorldResultCache(const MMatrix &prevTransform, const MMatrix &endBone) override;

    //CAUTION: NOT FINISHED!
    //in bone tight now nothing happens, the 
    //translation is not overriden by the updated translation.
    //root does.
    virtual void OnForwardPlueckerFinished() override {};
};