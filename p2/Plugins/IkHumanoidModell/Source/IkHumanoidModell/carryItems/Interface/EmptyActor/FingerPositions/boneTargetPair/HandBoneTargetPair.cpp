#include "HandBoneTargetPair.h"




void HandBoneTargetPair::SetMainTargetComponent(USceneComponent *mainComponentIn){
    mainTarget = mainComponentIn;
    if(mainComponentIn != nullptr){
        mainTargetOriginalLocation = mainComponentIn->GetRelativeLocation();
        mainTargetOriginalRotation = mainComponentIn->GetRelativeRotation();
    }
}

USceneComponent *HandBoneTargetPair::GetMainTargetComponent(){
    return mainTarget;
}

void HandBoneTargetPair::Update(USkeletalMeshComponent *compIn, FString boneNameIn){
    comp = compIn;
    boneName = boneNameIn;
}

void HandBoneTargetPair::UpdateAndEnable(USkeletalMeshComponent *compIn, FString boneNameIn){
    Update(compIn, boneNameIn);
    enableTemporaryTarget();
}


void HandBoneTargetPair::disableTemporaryTarget(){
    bIsEnabled = false;
}

void HandBoneTargetPair::enableTemporaryTarget(){
    bIsEnabled = true;
}

bool HandBoneTargetPair::temporaryTargetIsEnabled(){
    return bIsEnabled;
}

FVector HandBoneTargetPair::boneLocation(){
    if(temporaryTargetIsEnabled()){
        if(comp){
            //DebugHelper::logMessage("HandBoneTargetPair::NAME: ", boneName);
            return comp->GetBoneLocation(FName(*boneName));
        }
    }
    //world space!
    if(mainTarget){
        return mainTarget->GetComponentLocation();
    }

    FVector none(0, 0, 0);
    return none;
}
