#include "FingerBoneAttachment.h"
//Plugins/CoreMath/Source/CoreMath/Matrix/MMatrix.h

FingerBoneAttachment::FingerBoneAttachment(){

}

FingerBoneAttachment::~FingerBoneAttachment(){

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

