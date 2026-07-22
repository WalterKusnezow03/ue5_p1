// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "weapon.h"

#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "GameCore/team/teamEnum.h"
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"

#include "WingsuitInterface.h"
#include "GameCore/Input/InputContainer.h"
#include "GameCore/interfaces/Steeringinterface.h"



#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "p2/player/inventory/PlayerInventory.h"

//#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"
#include "GameCore/PlayerControllerBase/WidgetInteractionIncluded/PlayerControllerWidgetInteractiveBase.h"


#include "IkHumanoidModell/Ik/Controller/PlayerController/HumanoidPlayerController.h"
#include "CoreMath/animation/timer/Timer.h"



#include "playerScript.generated.h"

class APlateCarrier;

UCLASS()
class P2_API AplayerScript : public APlayerControllerWidgetInteractiveBase {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AplayerScript();


	void debugPathFinder();

	void reloadLoadout(LoadoutHelper &loadout);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PickupDefaultWeaponOnBeginPlay();
	void CreateUiHudActorOnBeginPlay();

	//derived from ACharacter
	virtual void Jump() override;

	//tracks input for steering vehicles, and unseat
	//InputContainer playerInputContainer;

	///overriden from base class!
	virtual bool IsPaused() override;
	bool InteractionBlockedPauseAndAnimation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//receive payloads from widgets
	virtual void ReceiveCallback(UWidgetInteractPayload *payload) override;

protected:
	bool isCamInPlayer = true;
	void switchCamera();
	FVector cameraReltiveLocationOriginal;

	void cameraDefaultFpv();
	void cameraDebugFpv();

	PlayerInventory playerInventory;


	// Pointer to the Skeletal Mesh Component
    class USkeletalMeshComponent* SkeletalMeshComponent;

	


	virtual void MoveForward(float Value) override;
    virtual void MoveRight(float Value) override;
    
	virtual void interact() override;
	virtual void reload() override;
	virtual void drop() override;
	virtual void aim() override;

	void TickWeaponSway();

	void shoot();
	void ApplyRecoil();

	void keydown1();
	void keydown2();
	void keydown3();
	void keydown4();
	void switchToIndex(int index);



	//BONE CONTROLLER
	void TickBoneController(float DeltaTime);
	void resetFlagsOnTick();

	
	class HumanoidPlayerController boneController;
	void setupBoneController();


	//WINGSUIT
	void addWingsuitVelocity(float DeltaTime);
	void TickUpdateWingsuit(float DeltaTime);
	void setWingsuitTimerOnMovement();
	class WingsuitInterface wingsuitInterface;
	static const int WINGSUIT_SPEED = 600; //6ms


	//UI
	
	virtual void openPauseMenu() override;
	void updateUi();
	void updateAmmunitionUi();
	void updateHealthUi();
	void updateInventoryUi();

	//Loadout setup
	void pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(
		std::vector<Aweapon *> &newWeapons
	);
	void pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(
		Aweapon *weapon
	);




	//DEBUG

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	void DebugTickPathfinderNN(float deltatime);


	//plate carrier
	void SetupPlateCarrierOnBeginPlay();
	APlateCarrier *plateCarrier = nullptr;
};
