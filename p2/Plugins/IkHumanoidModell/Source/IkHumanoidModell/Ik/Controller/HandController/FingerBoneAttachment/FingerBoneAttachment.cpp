#include "FingerBoneAttachment.h"
//Plugins/CoreMath/Source/CoreMath/Matrix/MMatrix.h

FingerBoneAttachment::FingerBoneAttachment(){

}

FingerBoneAttachment::~FingerBoneAttachment(){

}

void FingerBoneAttachment::setupBone(FHandFingerProperty &property){
    /*BoneAttachment::setupBone(
        property.GetSizeFirst(), 
        property.GetSizeSecond(), 
        property.GetWorld(), 
        property.GetPivot(), 
        property.GetWidth()
    );*/
    BoneAttachment::setupBoneBase(property, property.GetPivot());
    //setInnerOffsetTranslation(property.GetPivot());
}


void FingerBoneAttachment::OverrideTarget(FingerTargetPair &targetIn){
    target = targetIn;
}

void FingerBoneAttachment::TickAutoBasedOnTarget(MMatrix &transform, float deltatime){

    if(target.bIsEnabled()){
        setForwardTargetLocal(target.GetTarget());
        TickForwardKinematic(transform, deltatime);
    }else{
        TickNone(transform, deltatime);
    }
}



