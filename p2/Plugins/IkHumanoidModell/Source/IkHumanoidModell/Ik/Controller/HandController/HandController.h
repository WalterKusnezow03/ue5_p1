#pragma once

#include "CoreMinimal.h"

//#include "IkHumanoidModell/Ik/Controller/HipController/BoneAttachment.h"
#include "IkHumanoidModell/Ik/Controller/HandController/FingerBoneAttachment/FingerBoneAttachment.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"


class FHandProperty;
class FHumanoidControllerSetupPackage;

class IKHUMANOIDMODELL_API HandController {

public:
    HandController();
    ~HandController();

   

    void setup(FHumanoidControllerSetupPackage &package, EArmType type);
    void setup(FHandProperty &property, EArmType type);

    //update if item has changed
    void Update(IIkCarryInterface *item);
    void Update(CarriedItemPositionData &data);

    void ClearFingers();

    //Tick location update
    void Tick(MMatrix &transform, float deltatime);

    void getActors(TArray<AActor *> &outArray);
    

private:
    //finger create
    void CreateFingers(FHandProperty &property, EArmType type);
    void CreateFingers(FHandProperty &property, TArray<FHandFingerProperty> &fingerProperties);
    // finger create

    void CreatePalm(FHandProperty &property);
    void CreatePalm(UWorld *world);
    AActor *palm = nullptr;
    void TickPalm(MMatrix &transform);

    /*BoneAttachment f1; //thumb
    BoneAttachment f2; //pointer
    BoneAttachment f3; //middle
    BoneAttachment f4; //ring
    BoneAttachment f5; //small finger*/

    TArray<FingerBoneAttachment> fingers;
    UWorld *worldPointer = nullptr;

    MMatrix orientation;

    MMatrix debugOffset;

    EArmType typeSaved;

    bool bHasTargetSetup = false;

    void TickAutoBasedOnTarget(MMatrix &transform, float deltatime);
    void TickNone(MMatrix &transform, float deltatime);
    void TickForwardKinematic(MMatrix &transform, float deltatime);

    void DrawAxis(MMatrix &transform, FVector &location);
};