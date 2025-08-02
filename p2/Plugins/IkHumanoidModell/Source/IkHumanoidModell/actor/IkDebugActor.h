#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"
#include "IkHumanoidModell/Ik/Controller/HipController/HipController.h"
#include "CoreMath/animation/TargetInterpolator.h"

#include "IkDebugActor.generated.h"

UCLASS()
class IKHUMANOIDMODELL_API AIkDebugActor : public AActor {
    GENERATED_BODY()

public:
    static void CreateInstance(UWorld *world);

    AIkDebugActor();
    void BeginPlay() override;
    void Tick(float deltatime) override;

private:
    //new
    HipController hipController;

    TwoJointBone bone;
    TargetInterpolator interpolator;
    TargetInterpolator interpolator2;

    FVector start;
    FVector end;
    float time;

    bool flipFlag = false;
    FVector start1;
    FVector end1;

    void resetInterpolators();
};