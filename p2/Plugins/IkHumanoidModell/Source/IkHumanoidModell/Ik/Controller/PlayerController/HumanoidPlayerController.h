#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/BaseController/HumanoidController.h"

class UCameraComponent;

///@brief custom DERIVED HUMANOID controller for the first person player
/// doesnt need all of the features and maybe has a extended feature set
/// --- TODO: Think about making derivation private here ---
class IKHUMANOIDMODELL_API HumanoidPlayerController : public HumanoidController {

private:
    bool bDebugBlockPlayerApiCarriedItem = false;

public:
    HumanoidPlayerController();
    virtual ~HumanoidPlayerController();

    virtual void defaultSetup(UWorld *world) override;

    virtual void Tick(float deltatime) override;

    void OverrideTransformAndCamera(
        UCameraComponent &camera,
        FVector &location,
        FRotator &rotation
    );

    // -- not implemented yet: copied from 1.0 Bone Controller --
    void openWingsuit() {};
    void closeWingsuit() {};
    
    
    void weaponAimDownSight(bool aiming);
    
    ///@brief also known as "Hochanschlag"
    void weaponRaisedReadyPosition(bool raise);
    void weaponContactPosition();

private:
    void DebugDisableCollisionOnLimbs();

    void UpdateAttachmentSockets(UCameraComponent &camera);
    void extractRotation(UCameraComponent &camera);
    void extractTranslation(UCameraComponent &camera);

    virtual void TickMainCarriedItemSocket(float deltatime) override;

    MMatrix cameraRotationMatrix;
    MMatrix cameraWorldLocation;

    MMatrix hipTDebug;
    MMatrix hipRDebug;

    bool readyRaisedPositionFlagged = false;
};