#include "HandBoneTargetPair.h"

HandBoneTargetPair::HandBoneTargetPair(const HandBoneTargetPair &other){
    if(this != &other){
        *this = other;
    }
}

HandBoneTargetPair &HandBoneTargetPair::operator=(const HandBoneTargetPair &other){
    if(this != &other){
        comp = other.comp;
        boneName = other.boneName;
        bIsEnabled = other.bIsEnabled;
    }
    return *this;
}

void HandBoneTargetPair::Update(USkeletalMeshComponent *compIn, FString boneNameIn){
    comp = compIn;
    boneName = boneNameIn;
}

void HandBoneTargetPair::UpdateAndEnable(USkeletalMeshComponent *compIn, FString boneNameIn){
    Update(compIn, boneNameIn);
    enable();
}

void HandBoneTargetPair::disable(){
    bIsEnabled = false;
}

void HandBoneTargetPair::enable(){
    bIsEnabled = true;
}

bool HandBoneTargetPair::isEnabled(){
    return bIsEnabled;
}

FVector HandBoneTargetPair::boneLocation(){
    if(comp){
        //DebugHelper::logMessage("HandBoneTargetPair::NAME: ", boneName);
        return comp->GetBoneLocation(FName(*boneName));
    }
    FVector none(0, 0, 0);
    return none;
}
