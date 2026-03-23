#pragma once
// Copyright Walter Kusnezow All Rights Reserved.

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/BaseController/HumanoidController.h"
#include "IkHumanoidModell/Ik/Controller/PlayerController/playerHipController/PlayerHipController.h"

class UCameraComponent;

///@brief custom DERIVED HUMANOID controller for the first person player
/// doesnt need all of the features and maybe has a extended feature set
/// --- TODO: Think about making derivation private here ---
class IKHUMANOIDMODELL_API HumanoidPlayerController : public HumanoidController {

private:
    bool bDebugBlockPlayerApiCarriedItem = false;
    bool bLogMessage = false;

public:
    HumanoidPlayerController();
    virtual ~HumanoidPlayerController();

    virtual void defaultSetup(AActor *actor) override;

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


protected:
    virtual TArray<AActor *> actorInChildrenArray() override;

private:
    void DebugDisableCollisionOnLimbs();

    void MarkEmptyActorOwnedByPlayer();

    void UpdateAttachmentSockets(UCameraComponent &camera);
    void extractRotation(UCameraComponent &camera);
    void extractTranslation(UCameraComponent &camera);

    //overriden for camera rotation inject to item socket call tick
    virtual void TickMainCarriedItemSocket(float deltatime) override;

    MMatrix cameraRotationMatrix;

    FRotator cameraRotationLocalPitch;
    MMatrix cameraWorldLocation;

    MMatrix hipTranslationCopy;
    MMatrix hipRotationCopy;
    MMatrix &TranslationMatrixForSocket();

    bool readyRaisedPositionFlagged = false;




    //needed for custom leg interpolation!
    PlayerHipController playerHipController;


    //override throwing direction
    void OverrideItemThrowingDirection(UCameraComponent &camera);
   
};