#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
#include <map>
#include "CoreMath/Matrix/MMatrix.h"
#include "IkHumanoidModell/Ik/Controller/HandController/gesture/HandTarget.h"

class IKHUMANOIDMODELL_API CarriedItemPositionData {

public:
    CarriedItemPositionData();
    ~CarriedItemPositionData();

    void updateHandTargets(
        FVector rightHand,
        FVector leftHand
    );

    void updateHandTarget(
        FVector hand,
        EArmType type
    );

    void updateHandTarget(
        USceneComponent *hand,
        EArmType type
    );

    FVector rightHandWorld();
    FVector leftHandWorld();



    FQuat quatRotation(EArmType type);
    MMatrix GetRotationMatrix(EArmType type);

    TArray<FingerTargetPair *> GetFingerTargets(EArmType type);

private:

    void DrawAxis(USceneComponent *comp);
    void DrawAxis(FRotator r, FVector pos, UWorld *worldPointer);


public:
    //NEW
    HandTarget &GetHandTargetInfo(EArmType typearm);
    void UpdateHandAndFingersWorld(
        EArmType type,
        FVector handWorld,
        TArray<USceneComponent *> &components,
        USceneComponent *relativeHandomponent // relartive rotation for hand.
    );

private:
    std::map<EArmType, HandTarget> hands;
};