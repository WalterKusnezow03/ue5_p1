#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "IkHumanoidModell/Ik/Controller/HipController/HipController.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/TorsoController.h"
#include "IkHumanoidModell/Ik/CarriedItem/CarriedItemSocket.h"


/// @brief will control the skelleton, hip and torso controller
class IKHUMANOIDMODELL_API HumanoidController {

public:
    HumanoidController();
    ~HumanoidController();

    
    void defaultSetup(UWorld *world);
    void Tick(float deltatime);
    void attachOrReplaceCarriedItem(IIkCarryInterface *ptr);
    void dropCarriedItem();

    //rotation change of hipcontroller
    void LookAt(FVector &location);

    void setupRotationForNextStep(float radianYaw);
    void forceYawAdd(float degree);

    FVector GetLocation();
    FVector lookDirection();

    /// @brief will place skelleton at a target, foot on ground
    void SetLocation(FVector &target);

    void setStateWalking();
    void stopLocomotion();

private:

    //legs, locomotion
    HipController hipController;

    //arms and torso
    TorsoController torsoController;


    //main attached weapon / item
    CarriedItemSocket mainItemSocket;
    void TickMainCarriedItemSocket(float deltatime);
};