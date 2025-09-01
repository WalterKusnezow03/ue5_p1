#pragma once 
#include "CoreMinimal.h"
#include "CoreMath/animation/InterpolatorBase/TInterpolator.h"

class COREMATH_API FRotatorInterpolator : public TInterpolator<FRotator> {

public:
    FRotatorInterpolator();
    virtual ~FRotatorInterpolator();

    virtual float Distance(FRotator &a, FRotator &b) override;

    
    virtual FRotator interpolation(FRotator &fromIn, FRotator &toIn, float skalarCurrent) override;

    virtual bool endReached() override {
        return skalar() >= 1.0f;
    }

protected:

    float shorterAngleSum(FRotator &a, FRotator &b);
    float rotationDirectionShorter(float a, float b);
    float wrapAngle180(float angle);
};