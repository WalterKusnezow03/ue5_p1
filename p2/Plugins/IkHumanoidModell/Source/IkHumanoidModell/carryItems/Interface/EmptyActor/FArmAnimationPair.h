#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
#include "CoreMath/animation/KeyFrames/KeyFrameAnimation.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/EArmAnimationEnum.h"
#include "CoreMath/Matrix/MMatrix.h"

class UArmKeyFrameAnimationAsset;

//will store animation pair for arms, to be played by AIKCarryInterfaceAnimatedActor
//all animations should be local
class IKHUMANOIDMODELL_API FArmAnimationPair {

public:
    FArmAnimationPair();
    ~FArmAnimationPair();

    FArmAnimationPair(const FArmAnimationPair &other);
    FArmAnimationPair &operator=(const FArmAnimationPair &other);

    bool Tick(
        EArmType type, 
        float deltatime, 
        MMatrix &transform,
        FVector &outPosLocal
    );

    void SetupFrom(const UArmKeyFrameAnimationAsset *animationAsset);

    void SetAnimation(EArmType type, KeyFrameAnimation &animation);

    //dont add other animations, reference might get invalid!
    KeyFrameAnimation &KeyFrameAnimationReference(EArmType type);

    void SetType(EArmAnimationEnum typeIn);
    bool IsType(EArmAnimationEnum typeIn);

    //sets loop on all animations
    void SetLooping(bool flag);

    void ResetAnimations();

    void LogInfo();
    FString ToString();

    //custom axis constraint for animation
    FIKCarryInterfaceAxisConstraint &getAxisConstraint(){
        return axisConstraint;
    }

private:
    EArmAnimationEnum typeSaved = EArmAnimationEnum::none;

    bool singleFire = false;
    bool releaseActorOnEnd = false;

    //TODO!
    //create copy mechanism if animation changed
    //attach actor ejected or not. As FStrcut pair.
    //std::map<EArmType, AActor *> attachedActors;


    //keyframe animation needs local velocity
    std::map<EArmType, KeyFrameAnimation> animationMap;



    //rotation axis restraint for carried object / local animation
    FIKCarryInterfaceAxisConstraint axisConstraint;

    void CreateEmptyAnimation(EArmType type);
    void CreateEmptyAnimationsFor(const TArray<EArmType> &types);
};