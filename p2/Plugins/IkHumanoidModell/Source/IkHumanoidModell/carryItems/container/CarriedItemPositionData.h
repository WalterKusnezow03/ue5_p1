#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
#include <map>

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


    void UpdateHandAndFingersWorld(
        EArmType type, 
        FRotator &actorRotation,
        FVector &handWorld, 
        TArray<FVector> &array
    );
    
    //if is direct child of parent actor (hand location, no additional rotation.)
    //Uscene component is also compatible with bone locaition here!
    void UpdateHandAndFingersWorld(
        EArmType type,
        AActor *owningActor,
        FVector worldHand,
        TArray<USceneComponent *> &components
    );

    //update finge targets
    void UpdateFingersLocal(EArmType type, TArray<FVector> &array);
    TArray<FVector> &GetFingerTargetsLocal(EArmType type);

private:
    FVector rightHandWorldSaved;
    FVector leftHandWorldSaved;

    std::map<EArmType, TArray<FVector>> fingerPositions;

};