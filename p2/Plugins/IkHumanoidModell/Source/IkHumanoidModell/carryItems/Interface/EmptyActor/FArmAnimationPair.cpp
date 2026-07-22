#include "FArmAnimationPair.h"
#include "DebugPlugin/DebugHelper.h"

#include "IkHumanoidModell/carryItems/Interface/AnimationAsset/ArmKeyFrameAnimationAsset.h"

FArmAnimationPair::FArmAnimationPair(){

}

FArmAnimationPair::~FArmAnimationPair(){

}

FArmAnimationPair::FArmAnimationPair(const FArmAnimationPair &other){
    if(this != &other){
        *this = other;
    }
}

FArmAnimationPair &FArmAnimationPair::operator=(const FArmAnimationPair &other){
    if(this != &other){
        animationMap = other.animationMap;
        typeSaved = other.typeSaved;
        axisConstraint = other.axisConstraint;
    }
    return *this;
}

void FArmAnimationPair::SetAnimation(EArmType type, KeyFrameAnimation &animation){
    animationMap[type] = animation;
}

void FArmAnimationPair::SetupFrom(const UArmKeyFrameAnimationAsset *animationAsset){
    if(animationAsset){
    
        //copy animations
        TArray<EArmType> keys = animationAsset->GetKeyList();
        CreateEmptyAnimationsFor(keys);
        for (int i = 0; i < keys.Num(); i++){
            EArmType currentKey = keys[i];
            KeyFrameAnimation &anim = KeyFrameAnimationReference(currentKey);
            animationAsset->CopyDataTo(currentKey, anim);
        }

        //copy properties
        //rotation axis restraint for carried object / local animation
        axisConstraint = animationAsset->axisConstraintForObject;

        //after copy of animation
        SetLooping(animationAsset->loopAnimation);
    }
}



void FArmAnimationPair::ResetAnimations(){
    for(auto &pair : animationMap){
        KeyFrameAnimation &current = pair.second;
        current.restart();
    }
}


void FArmAnimationPair::SetLooping(bool flag){
    for(auto &pair : animationMap){
        KeyFrameAnimation &current = pair.second;
        current.SetLoopFlag(flag);
    }
}


void FArmAnimationPair::CreateEmptyAnimationsFor(const TArray<EArmType> &types){
    for (int i = 0; i < types.Num(); i++){
        CreateEmptyAnimation(types[i]);
    }
}

void FArmAnimationPair::CreateEmptyAnimation(EArmType type){
    KeyFrameAnimation empty;
    SetAnimation(type, empty);
}

KeyFrameAnimation &FArmAnimationPair::KeyFrameAnimationReference(EArmType type){
    if(animationMap.find(type) == animationMap.end()){
        KeyFrameAnimation empty;
        SetAnimation(type, empty);
    }
    return animationMap[type];
}

bool FArmAnimationPair::Tick(
    EArmType type, 
    float deltatime,
    FVector &outPosLocal
){
    if(animationMap.find(type) != animationMap.end()){
        KeyFrameAnimation &current = animationMap[type];
        FVector local = current.interpolate(deltatime);
        outPosLocal = local;


        /// @brief never true, if animation marked looping
        bool resultFinishedAnimation = current.reachedLastFrameOfAnimation();
        return resultFinishedAnimation;
    }
    //true finished if not found.
    return true;
}

void FArmAnimationPair::SetType(EArmAnimationEnum typeIn){
    typeSaved = typeIn;
}


bool FArmAnimationPair::IsType(EArmAnimationEnum typeIn){
    return typeSaved == typeIn;
}


void FArmAnimationPair::ScaleToTime(float targetTime){
    if(HasAnimations()){
        //find max time animation
        float maxAnimationTime = LongestAnimationTime();
        float scalar = targetTime / maxAnimationTime; // distTarget / distAll
        ScaleTimeWithScalar(scalar);

        //LOG
        if(false){
            FString message = FString::Printf(
                TEXT("FArmAnimationPair::ScaleToTime %.2f with %.2f from max %.2f"),
                targetTime,
                scalar, 
                maxAnimationTime
            );
            DebugHelper::logMessage(message);
            DebugHelper::showScreenMessage(message);
            message = FString::Printf(
                TEXT("FArmAnimationPair::ScaleToTime after %.2f"),
                LongestAnimationTime()
            );
            DebugHelper::logMessage(message);
            DebugHelper::showScreenMessage(message);
        }
        
        // LOG


        
    }
}

void FArmAnimationPair::ScaleTimeWithScalar(float scalar){
    scalar = std::abs(scalar);
    for (auto &pair : animationMap){
        KeyFrameAnimation &currentAnimation = pair.second;
        currentAnimation.ScaleTimeWithScalar(scalar);
    }
}

float FArmAnimationPair::LongestAnimationTime(){
    float maxTime = 0.0f;
    for (auto &pair : animationMap)
    {
        const KeyFrameAnimation &currentAnimation = pair.second;
        float compareTime = currentAnimation.totalLength();
        maxTime = std::max(maxTime, compareTime);
    }
    return maxTime;
}

bool FArmAnimationPair::HasAnimations(){
    return animationMap.size() > 0;
}

void FArmAnimationPair::LogInfo(){
    DebugHelper::logMessage(ToString());
}

#include "IkHumanoidModell/carryItems/Interface/AnimationTableBp/IKAnimationAssetLoader.h"
FString FArmAnimationPair::ToString(){
    FString pairInfo = TEXT("FArmAnimationPair: ");
    pairInfo += IKAnimationAssetLoader::StaticAnimationToString(typeSaved);
    
    for(auto &pair : animationMap){
        KeyFrameAnimation &current = pair.second;
        pairInfo += current.ToString();
    }
    return pairInfo;
}

