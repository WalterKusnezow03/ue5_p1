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

FVector CarriedItemPositionData::rightHandWorld(){
    return rightHandWorldSaved;
}

FVector CarriedItemPositionData::leftHandWorld(){
    return leftHandWorldSaved;
}
