#pragma once

#include "CoreMinimal.h"

class PATHFINDERNNEXTENSION_API FVisionCone {

public:
    void Update(const FVector &pos, const FVector &lookDir, float angleIn);
    
    FVector &ActorLocation();
    FVector2D &GetLookDir();
    
    float GetAngle();

protected:
    FVector location;
    FVector2D lookDir2D;
    float angle = 0.0f;
};