#include "KeyFrameAnimationChained.h"


void KeyFrameAnimationChained::clear(){
    passedEnd = false;
    index = 0;
    animations.Empty();
}

bool KeyFrameAnimationChained::IsFinished(){
    if(passedEnd){
        passedEnd = false;
        return true;
    }

    return false;
}

void KeyFrameAnimationChained::UpdatePassedEndFlag(){
    if(index >= animations.Num()){
        passedEnd = true;
    }
}


void KeyFrameAnimationChained::AddAnimation(PayloadKeyFrameAnimation &animation){
    animations.Add(animation);
}

bool KeyFrameAnimationChained::TryTick(FVector &outPos, float deltatime){
    if(!IsFinished()){
        outPos = Tick(deltatime);
        return true;
    }
    return false;
}

FVector KeyFrameAnimationChained::Tick(float deltatime){
    FVector outpos;


    //tick this
    //check index 
    //notify if needed
    
    if(PayloadKeyFrameAnimation *current = CurrentAnimation()){
        outpos = current->interpolate(deltatime);
        //current->reachedLastFrameOfAnimation() ||
        if(current->PayloadExecuted()){
            index++;
            UpdatePassedEndFlag();
        }
    }

    return outpos;
}



PayloadKeyFrameAnimation *KeyFrameAnimationChained::CurrentAnimation(){
    if(index >= 0 && index < animations.Num()){
        return &animations[index];
    }
    return nullptr;
}


FString KeyFrameAnimationChained::Info(){
    FString outString = FString::Printf(TEXT("KeyFrameAnimationChained %d of %d"), index + 1, animations.Num());
    return outString;
}


FString KeyFrameAnimationChained::InfoExtended(){
    FString prefix = Info();
    if(PayloadKeyFrameAnimation *current = CurrentAnimation()){
        prefix += FString::Printf(TEXT(" -> %s"), *current->StatusInfo());
    }else{
        prefix += " -> ()";
    }
    return prefix;
}

TArray<FVector> KeyFrameAnimationChained::GetAllKeyFrames(){
    TArray<FVector> outArray;
    for (int i = 0; i < animations.Num(); i++){
        PayloadKeyFrameAnimation &current = animations[i];
        current.GetAllKeyFrames(outArray);
    }
    return outArray;
}



TArray<FVector> KeyFrameAnimationChained::GetAllKeyFrames(const FTransform &offset){
    TArray<FVector> outArray = GetAllKeyFrames();
    for (int i = 0; i < outArray.Num(); i++){
        outArray[i] = offset.TransformPosition(outArray[i]);
    }
    return outArray;
}




