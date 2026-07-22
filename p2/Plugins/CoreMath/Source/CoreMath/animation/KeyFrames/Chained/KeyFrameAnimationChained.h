#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/KeyFrames/Payloaded/PayloadKeyFrameAnimation.h"


//allows to chain animations and dispatch payloads once finished
//each of them.
class COREMATH_API KeyFrameAnimationChained{

public:
    void clear();

    bool IsFinished();

    //is COPIED!
    void AddAnimation(PayloadKeyFrameAnimation &animation);

    //tick all, auto switch to next animation
    bool TryTick(FVector &outPos, float deltatime);

    FString Info();
    FString InfoExtended();
    
    //debug
    TArray<FVector> GetAllKeyFrames();
    TArray<FVector> GetAllKeyFrames(const FTransform &offset);

private:
    //notifier happens interally 
    TArray<PayloadKeyFrameAnimation> animations; 
    int index = 0;
    bool passedEnd = false;

    PayloadKeyFrameAnimation *CurrentAnimation();
    FVector Tick(float deltatime);

    void UpdatePassedEndFlag();
};