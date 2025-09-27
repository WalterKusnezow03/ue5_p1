#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"
#include "IkHumanoidModell/Ik/Controller/HipController/HipController.h"
#include "IkHumanoidModell/Ik/Controller/BaseController/HumanoidController.h"
#include "CoreMath/animation/TransformInterpolator.h"
#include "IkHumanoidModell/actor/debugEnum/EDebugPart.h"

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
    void BeginPlaySingleArm();
    void BeginPlayHipController();
    void BeginPlayHumanoidController();
    

    void TickPlaySingleArm(float deltatime);
    void TickPlayHipController(float deltatime);
    void TickPlayHumanoidController(float deltatime);

    //EDebugPart debugPart = EDebugPart::EDebugHipController; 
    EDebugPart debugPart = EDebugPart::EDebugHumanoidController;

    HipController hipController;

    //new
    HumanoidController humanController;

    TwoJointBone bone;
    TransformInterpolator interpolator;
    TransformInterpolator interpolator2;

    FVector start;
    FVector end;
    float time;

    bool flipFlag = false;
    FVector start1;
    FVector end1;

    void resetInterpolators();
};