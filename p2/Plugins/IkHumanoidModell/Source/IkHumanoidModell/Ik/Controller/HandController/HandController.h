#pragma once

#include "CoreMinimal.h"

#include "IkHumanoidModell/Ik/Controller/HipController/BoneAttachment.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"

class IKHUMANOIDMODELL_API HandController {

public:
    HandController();
    ~HandController();

    void setup(UWorld *world, EArmType type);

    //update if item has changed
    void Update(IIkCarryInterface *item);
    void Update(CarriedItemPositionData &data);

    void ClearFingers();

    //Tick location update
    void Tick(MMatrix &transform, float deltatime);

    void getActors(TArray<AActor *> &outArray);
    

private:
    /*BoneAttachment f1; //thumb
    BoneAttachment f2; //pointer
    BoneAttachment f3; //middle
    BoneAttachment f4; //ring
    BoneAttachment f5; //small finger*/

    TArray<BoneAttachment> fingers;
    UWorld *worldPointer = nullptr;

    MMatrix orientation;

    MMatrix debugOffset;

    EArmType typeSaved;

    bool bHasTargetSetup = false;

    void TickNone(MMatrix &transform, float deltatime);
    void TickForwardKinematic(MMatrix &transform, float deltatime);
};