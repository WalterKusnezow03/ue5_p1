#include "FingerTargetPair.h"

FingerTargetPair::FingerTargetPair(){
    bIsEnabledFlag = false;
}

FingerTargetPair::~FingerTargetPair(){

}

FingerTargetPair::FingerTargetPair(const FingerTargetPair &other){
    if(this != &other){
        *this = other;
    }
}

FingerTargetPair &FingerTargetPair::operator=(const FingerTargetPair &other){
    if(this != &other){
        localTarget = other.localTarget;
        bIsEnabledFlag = other.bIsEnabledFlag;
    }
    return *this;
}

void FingerTargetPair::OverrideTarget(FVector &target){
    localTarget = target;
}

FVector &FingerTargetPair::GetTarget(){
    return localTarget;
}

bool FingerTargetPair::bIsEnabled(){
    return bIsEnabledFlag;
}

void FingerTargetPair::enable(bool flag){
    bIsEnabledFlag = flag;
}
