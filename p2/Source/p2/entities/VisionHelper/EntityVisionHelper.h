#pragma once

#include "CoreMinimal.h"

//will manage dot product and 
//vision angle handle for entities
//is tested.
class P2_API FEntityVisionHelper {

public:
    FEntityVisionHelper();
    ~FEntityVisionHelper();
    void setup(float visionAngleDegreeMaxIn);

    bool DotProductWithinVisionAngle(float dotIn);
    float VisionAngleDegree();
    float VisionAngleDot();

protected:
    float visionAngleDegree = 0.0f;
    float visionAngleRad = 0.0f;
    float visionAngleDotMin = 0.0f; //relative to forward max dot product on normalized Vectors
};