#pragma once

#include "CoreMinimal.h"
#include <map>
#include "IkHumanoidModell/carryItems/Interface/ComponentTracking/HandBoneIndexEnum.h"
#include "IkHumanoidModell/Ik/Controller/HandController/gesture/FingerTargetPair.h"

//will store finger and hand locations and rotation for
//updating the handcontroller


class IKHUMANOIDMODELL_API HandTarget {

public:
    HandTarget();
    ~HandTarget();

    FingerTargetPair &FingerTargetAt(HandBoneIndexEnum finger);
    TArray<FingerTargetPair*> GetFingerTargets();

    void UpdateHand(FVector &locationIn);
    void UpdateHand(FVector &handLocationIn, FQuat &inQuat);
    void UpdateHand(
        TArray<USceneComponent *> &targets,
        FVector &handLocationIn,
        FQuat &rotationin
    );

    void UpdateFinger(
        FVector &localLocation,
        bool enabled,
        HandBoneIndexEnum finger
    );

    FVector GetHandTargetLocation();
    FQuat GetHandTargetQuatRotation();
    MMatrix GetHandTargetRotation();

private:
    FQuat handRotation;
    FVector handLocation;

    MMatrix handInverse;
    void UpdateHandTransformAndInverse(FVector &locationIn);
    void UpdateHandTransformAndInverse(FVector &locationIn, FQuat &rIn);
    void UpdateHandInverse();
    void MoveToLocalSpace(FVector &pos);

    HandBoneIndexEnum indexToEnum(int i);
    TArray<HandBoneIndexEnum> IndexEnumAsArray();

    std::map<HandBoneIndexEnum, FingerTargetPair> FingerTargets;

    bool IsValid(HandBoneIndexEnum finger);
};