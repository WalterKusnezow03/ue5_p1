#include "VisionCone.h"


void FVisionCone::Update(const FVector &pos, const FVector &lookDir, float angleIn){
    lookDir2D = FVector2D(lookDir.X, lookDir.Y).GetSafeNormal();
    angle = std::abs(angleIn);
    location = pos;
}

void FVisionCone::UpdateAs360(const FVector &pos){
    FVector look(1, 0, 0);
    Update(pos, look, 360.0f);
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
