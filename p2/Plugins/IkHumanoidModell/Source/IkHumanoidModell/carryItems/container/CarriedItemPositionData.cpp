#include "CarriedItemPositionData.h"

#include "DebugPlugin/DebugHelper.h"

CarriedItemPositionData::CarriedItemPositionData(){

}

CarriedItemPositionData::~CarriedItemPositionData(){

}

void CarriedItemPositionData::updateHandTargets(
    FVector rightHand,
    FVector leftHand
){
    updateHandTarget(rightHand, EArmType::ERight);
    updateHandTarget(leftHand, EArmType::ELeft);
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
    FVector handLocation,
    EArmType type
){
    HandTarget &target = GetHandTargetInfo(type);
    target.UpdateHand(handLocation);
}

FVector CarriedItemPositionData::rightHandWorld(){
    HandTarget &target = GetHandTargetInfo(EArmType::ERight);
    return target.GetHandTargetLocation();
}

FVector CarriedItemPositionData::leftHandWorld(){
    HandTarget &target = GetHandTargetInfo(EArmType::ELeft);
    return target.GetHandTargetLocation();
}





//debug method to debug hand rotation.
void CarriedItemPositionData::DrawAxis(USceneComponent *comp){
    if(comp){
        DrawAxis(comp->GetComponentRotation(), comp->GetComponentLocation(), comp->GetWorld());
    }
}

void CarriedItemPositionData::DrawAxis(FRotator r, FVector pos, UWorld *worldPointer){
    if(worldPointer){
        MMatrix orientation;
        orientation.setRotation(r);

        FVector x(5, 0, 0);
        FVector y(0, 5, 0);
        FVector z(0, 0, 5);

        x = orientation * x;
        y = orientation * y;
        z = orientation * z;

        DebugHelper::showLineBetween(worldPointer, pos, pos + x, FColor::Red, 0.3f);
        DebugHelper::showLineBetween(worldPointer, pos, pos + y, FColor::Green, 0.3f);
        DebugHelper::showLineBetween(worldPointer, pos, pos + z, FColor::Blue, 0.3f);
    }
}






/// ---------------- REFACTURE  ----------------

HandTarget &CarriedItemPositionData::GetHandTargetInfo(EArmType typearm){
    if(hands.find(typearm) == hands.end()){
        hands[typearm] = HandTarget();
    }
    return hands[typearm];
}


void CarriedItemPositionData::UpdateHandAndFingersWorld(
    EArmType type, 
    FVector handWorld, 
    TArray<USceneComponent*> &components,
    USceneComponent *relativeHandomponent //relartive rotation for hand.
){
    if(relativeHandomponent){
        FQuat rotation = relativeHandomponent->GetComponentRotation().Quaternion();
        HandTarget &hand = GetHandTargetInfo(type);
        hand.UpdateHand(
            components,
            handWorld,
            rotation
        );
    }
    
}


FQuat CarriedItemPositionData::quatRotation(EArmType type){
    HandTarget &hand = GetHandTargetInfo(type);
    return hand.GetHandTargetQuatRotation();
}

MMatrix CarriedItemPositionData::GetRotationMatrix(EArmType type){
    HandTarget &hand = GetHandTargetInfo(type);
    return hand.GetHandTargetRotation();
}


//MUST BE REFACTURED!

TArray<FingerTargetPair*> CarriedItemPositionData::GetFingerTargets(EArmType type){
    HandTarget &target = GetHandTargetInfo(type);
    return target.GetFingerTargets();
}