#include "VisionCone.h"


void FVisionCone::Update(const FVector &pos, const FVector &lookDir, float angleIn){
    lookDir2D = FVector2D(lookDir.X, lookDir.Y).GetSafeNormal();
    angle = std::abs(angleIn);
    location = pos;
}

FVector &FVisionCone::ActorLocation(){
    return location;
}

FVector2D &FVisionCone::GetLookDir(){
    return lookDir2D;
}

float FVisionCone::GetAngle(){
    return angle;
}
