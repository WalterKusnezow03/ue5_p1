#include "CarriedItemPositionData.h"

CarriedItemPositionData::CarriedItemPositionData(){

}

CarriedItemPositionData::~CarriedItemPositionData(){

}

void CarriedItemPositionData::updateHandTargets(
    FVector rightHand,
    FVector leftHand
){
    rightHandWorldSaved = rightHand;
    leftHandWorldSaved = leftHand;
}


void CarriedItemPositionData::updateHandTarget(
    USceneComponent *hand,
    EArmType type
){
    if(hand){
        updateHandTarget(hand->GetComponentLocation(), type);
    }
}


void CarriedItemPositionData::updateHandTarget(
    FVector hand,
    EArmType type
){
    if(type == EArmType::ERight){
        rightHandWorldSaved = hand;
    }
    if(type == EArmType::ELeft){
        leftHandWorldSaved = hand;
    }
}



FVector CarriedItemPositionData::rightHandWorld(){
    return rightHandWorldSaved;
}

FVector CarriedItemPositionData::leftHandWorld(){
    return leftHandWorldSaved;
}




void CarriedItemPositionData::UpdateFingersLocal(EArmType type, TArray<FVector> &array){
    TArray<FVector> &ref = GetFingerTargetsLocal(type);
    ref = array; //calls copy constructor.
}

TArray<FVector> &CarriedItemPositionData::GetFingerTargetsLocal(EArmType type){
    if(fingerPositions.find(type) == fingerPositions.end()){
        fingerPositions[type] = TArray<FVector>();
    }
    return fingerPositions[type];
}


void CarriedItemPositionData::UpdateHandAndFingersWorld(
    EArmType type, 
    FRotator &actorRotation,
    FVector &handWorld, 
    TArray<FVector> &array
){
    updateHandTarget(handWorld, type);

    //M = T * R
    //M^-1 = R^-1 * T ^-1
    FVector v1 = handWorld * -1.0f;
    MMatrix t1(v1);

    MMatrix r(actorRotation);
    MMatrix r1 = r.transposedRotation();
    MMatrix m1 = r1 * t1;

    //testing needed!!
    for(FVector &current : array){
        current = m1 * current;
    }
    UpdateFingersLocal(type, array);
}

void CarriedItemPositionData::UpdateHandAndFingersWorld(
    EArmType type, 
    AActor *owningActor,
    FVector handWorld, 
    TArray<USceneComponent*> &components
){
    
    if(owningActor){
        FRotator r = owningActor->GetActorRotation();
        TArray<FVector> fingerLocations;
        fingerLocations.SetNum(components.Num());
        for (int i = 0; i < fingerLocations.Num(); i++){
            if(USceneComponent *current = components[i]){
                fingerLocations[i] = current->GetComponentLocation();
            }
        }
        UpdateHandAndFingersWorld(type, r, handWorld, fingerLocations);
    }
}