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
    /*if(itemIsAttached()){
        MMatrix transformInnerModified = AddTemporaryIIkCarryInterfaceAnimationOffsetToInnerTransformOnTick();

        //M = T * R * M_inner
        MMatrix TR = translation * orientation; //<--lese richtung--
        //MMatrix M = TR * TransformInner;
        MMatrix M = TR * transformInnerModified;

        FVector updateLocation = M.getTranslation();
        FRotator updateRotator = M.extractRotator();

        //apply rotation constraints given from item 
        FIKCarryInterfaceAxisConstraint &constraint = attachedItem->getAxisConstraint();
        constraint.ApplyAxisConstraint(updateRotator);


        attachedItem->UpdateActorTransform(updateLocation, updateRotator);
    }*/

    FRotator empty;
    Tick(
        deltatime,
        translation,
        orientation, // orientation of skelleton
        empty        // orientation of camera
    );
}

void CarriedItemSocket::Tick(
    float deltatime,
    MMatrix &translation,
    MMatrix &orientationRaw, //orientation of skelleton
    FRotator &orientationCamera //orientation of camera
){
    if(itemIsAttached()){
        
        //apply rotation constraints given from item 
        FIKCarryInterfaceAxisConstraint &constraint = attachedItem->getAxisConstraint();
        constraint.ApplyAxisConstraint(orientationCamera);
        MMatrix rCameraMat;
        rCameraMat.setRotation(orientationCamera);
        MMatrix RcombinedOrientatationConstrainted = orientationRaw * rCameraMat;
        




        MMatrix transformInnerModified = AddTemporaryIIkCarryInterfaceAnimationOffsetToInnerTransformOnTick();

        //M = T * R * M_inner
        MMatrix TR = translation * RcombinedOrientatationConstrainted; //<--lese richtung--
        //MMatrix M = TR * TransformInner;
        MMatrix M = TR * transformInnerModified;

        FVector updateLocation = M.getTranslation();
        FRotator updateRotator = M.extractRotator();

        //apply rotation constraints given from item 
        constraint.ApplyAxisConstraint(updateRotator);


        attachedItem->UpdateActorTransform(updateLocation, updateRotator);
    }
}





MMatrix CarriedItemSocket::AddTemporaryIIkCarryInterfaceAnimationOffsetToInnerTransformOnTick(){
    if (itemIsAttached()){
        MMatrix copy = TransformInner;
        FVector offset = copy.getTranslation();

        //constraint position for socket might be wanted.
        FIKCarryInterfaceAxisConstraint &constraint = attachedItem->getAxisConstraint();
        constraint.ApplyPositionConstraint(offset);


        offset += attachedItem->LocalAnimationOffset();
        copy.setTranslation(offset);
        return copy;
    }
    return TransformInner;
}

/// @brief local translation when updating the socket and carried item position and rotation
/// @param offset 
void CarriedItemSocket::setLocalLocation(FVector &offset){
    //M = T * R <-- lese richtung -- deshalb darf translation rein kopiert werden!
    TransformInner.setTranslation(offset);
}


void CarriedItemSocket::setLocalRotation(FRotator &rotation){
    //M = T * R <-- lese richtung --, deshlab darf rotation reinkopiert werden!
    TransformInner.setRotation(rotation);
}


IIkCarryInterface *CarriedItemSocket::attachedItemPointer(){
    return attachedItem;
}