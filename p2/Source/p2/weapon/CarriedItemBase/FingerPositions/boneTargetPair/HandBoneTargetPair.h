#pragma once

#include "CoreMinimal.h"

class P2_API HandBoneTargetPair {
public:
    HandBoneTargetPair(){
        bIsEnabled = false;
    }
    ~HandBoneTargetPair(){}
    HandBoneTargetPair(const HandBoneTargetPair &other);
    HandBoneTargetPair &operator=(const HandBoneTargetPair &other);

    void Update(USkeletalMeshComponent *compIn, FString boneNameIn);
    void UpdateAndEnable(USkeletalMeshComponent *compIn, FString boneNameIn);
    void disable();
    void enable();
    bool isEnabled();

    FVector boneLocation();

private:
    bool bIsEnabled;
    USkeletalMeshComponent *comp = nullptr;
    FString boneName;
};