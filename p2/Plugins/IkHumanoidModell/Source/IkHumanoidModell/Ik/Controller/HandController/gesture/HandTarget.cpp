#include "HandTarget.h"

HandTarget::HandTarget(){}
HandTarget::~HandTarget(){}



TArray<FingerTargetPair*> HandTarget::GetFingerTargets(){
    TArray<FingerTargetPair *> outArray;
    TArray<HandBoneIndexEnum> indexArray = IndexEnumAsArray();
    for (int i = 0; i < indexArray.Num(); i++){
        FingerTargetPair &current = FingerTargetAt(indexArray[i]);
        outArray.Add(&current);
    }
    return outArray;
}

FingerTargetPair &HandTarget::FingerTargetAt(HandBoneIndexEnum finger){
    if(FingerTargets.find(finger) == FingerTargets.end()){
        FingerTargets[finger] = FingerTargetPair();
    }
    return FingerTargets[finger];
}


void HandTarget::UpdateHand(FVector &handLocationIn){
    UpdateHandTransformAndInverse(handLocationIn);
}


void HandTarget::UpdateHand(FVector &handLocationIn, FQuat &inQuat){
    UpdateHandTransformAndInverse(handLocationIn, inQuat);
}


void HandTarget::UpdateHand(
    TArray<USceneComponent *> &targets, 
    FVector &handLocationIn, 
    FQuat &rotationin
){
    UpdateHandTransformAndInverse(handLocationIn, rotationin);
    for (int i = 0; i < targets.Num(); i++){
        HandBoneIndexEnum finger = indexToEnum(i);
        
        bool enable = false;
        FVector location;
        if (USceneComponent *current = targets[i])
        {
            enable = true;
            location = current->GetComponentLocation();
        }
        MoveToLocalSpace(location);
        UpdateFinger(
            location,
            enable,
            finger
        );
    }
}

void HandTarget::MoveToLocalSpace(FVector &pos){
    pos = handInverse * pos;
}

void HandTarget::UpdateFinger(
    FVector &localLocation,
    bool enabled,
    HandBoneIndexEnum finger
){
    if(IsValid(finger)){
        FingerTargetPair &pair = FingerTargetAt(finger);
        pair.enable(enabled);
        if(enabled){
            pair.OverrideTarget(localLocation);
        }
    }
   
}



void HandTarget::UpdateHandTransformAndInverse(FVector &locationIn, FQuat &rIn){
    handRotation = rIn;
    handLocation = locationIn;
    UpdateHandInverse();
}

void HandTarget::UpdateHandTransformAndInverse(FVector &locationIn){
    handLocation = locationIn;
    UpdateHandInverse();
}

void HandTarget::UpdateHandInverse(){
    //M = T * R <-- lese richtung
    //M^-1 = R^T * T^-1
    MMatrix t1;
    FVector _t1 = handLocation * -1.0f;
    t1.setTranslation(_t1);

    MMatrix rT;
    rT.setRotation(handRotation);
    rT.transposeRotation();

    handInverse = rT * t1; 
}

FVector HandTarget::GetHandTargetLocation(){
    return handLocation;
}

FQuat HandTarget::GetHandTargetQuatRotation(){
    return handRotation;
}

MMatrix HandTarget::GetHandTargetRotation(){
    MMatrix r;
    r.setRotation(handRotation);
    return r;
}


TArray<HandBoneIndexEnum> HandTarget::IndexEnumAsArray(){
    TArray<HandBoneIndexEnum> outArray = {
        HandBoneIndexEnum::thumb,
        HandBoneIndexEnum::finger1,
        HandBoneIndexEnum::finger2,
        HandBoneIndexEnum::finger3,
        HandBoneIndexEnum::finger4
    };
    return outArray;
}

HandBoneIndexEnum HandTarget::indexToEnum(int i){
    switch(i){
        case 0: return HandBoneIndexEnum::thumb;
        case 1: return HandBoneIndexEnum::finger1;
        case 2: return HandBoneIndexEnum::finger2;
        case 3: return HandBoneIndexEnum::finger3;
        case 4: return HandBoneIndexEnum::finger4;
    }
    return HandBoneIndexEnum::none;
}

bool HandTarget::IsValid(HandBoneIndexEnum finger){
    TArray<HandBoneIndexEnum> validArray = IndexEnumAsArray();
    return validArray.Contains(finger);
}
