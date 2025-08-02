#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "IkHumanoidModell/Ik/Controller/HipController/HipController.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/TorsoController.h"


/// @brief will control the skelleton, hip and torso controller
class IKHUMANOIDMODELL_API HumanoidController {

public:
    HumanoidController();
    ~HumanoidController();

    void defaultSetup(UWorld *world);
    void Tick(float deltatime);
    void attachOrReplaceCarriedItem(IIkCarryInterface *ptr);

private:

    //legs, locomotion
    HipController hipController;

    //arms and torso
    TorsoController torsoController;
};