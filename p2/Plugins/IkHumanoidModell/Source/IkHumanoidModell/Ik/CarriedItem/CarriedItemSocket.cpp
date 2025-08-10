#include "CarriedItemSocket.h"

CarriedItemSocket::CarriedItemSocket(){
    attachedItem = nullptr;
}

CarriedItemSocket::~CarriedItemSocket(){
    attachedItem = nullptr;
}

bool CarriedItemSocket::itemIsAttached(){
    return attachedItem != nullptr;
}

void CarriedItemSocket::attachOrReplaceCarriedItem(IIkCarryInterface *itemIn){
    attachedItem = itemIn;
}

void CarriedItemSocket::dropCarriedItem(){
    attachedItem = nullptr;
}

void CarriedItemSocket::Tick(
    float deltatime,
    MMatrix &translation, //actor translation world
    MMatrix &orientation //orientation of actor or even combined with limb or camera look direction.
){
    if(itemIsAttached()){
        //M = T * R * M_inner
        MMatrix TR = translation * orientation; //<--lese richtung--
        MMatrix M = TR * TransformInner;

        FVector updateLocation = M.getTranslation();
        FRotator updateRotator = M.extractRotator();

        attachedItem->UpdateActorTransform(updateLocation, updateRotator);
    }
}

/// @brief local translation when updating the socket and carried item position and rotation
/// @param offset 
void CarriedItemSocket::setLocalLocation(FVector &offset){
    TransformInner.setTranslation(offset);
}


void CarriedItemSocket::setLocalRotation(FRotator &rotation){
    TransformInner.setRotation(rotation);
}


IIkCarryInterface *CarriedItemSocket::attachedItemPointer(){
    return attachedItem;
}