#pragma once

#include "CoreMinimal.h"

//allows to set main target component and temporary target
//via skeletal mesh comp name and bone name
class IKHUMANOIDMODELL_API HandBoneTargetPair {

protected:
    USceneComponent *mainTarget = nullptr;
    FVector mainTargetOriginalLocation;
    FRotator mainTargetOriginalRotation;

public:
    void SetMainTargetComponent(USceneComponent *mainComponentIn);
    USceneComponent *GetMainTargetComponent();

    void Update(USkeletalMeshComponent *compIn, FString boneNameIn);
    void UpdateAndEnable(USkeletalMeshComponent *compIn, FString boneNameIn);
    void disableTemporaryTarget();
    void enableTemporaryTarget();
    bool temporaryTargetIsEnabled();

    FVector boneLocation();

private:
    bool bIsEnabled = false;
    USkeletalMeshComponent *comp = nullptr;
    FString boneName;
};