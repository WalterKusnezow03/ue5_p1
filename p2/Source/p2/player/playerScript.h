// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "weapon.h"
#include "playerInventory.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "GameCore/team/teamEnum.h"
#include "GameCore/interfaces/Damageinterface.h"
#include "GameCore/util/timer.h"
#include "WingsuitInterface.h"
#include "GameCore/Input/InputContainer.h"
#include "GameCore/interfaces/Steeringinterface.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "p2/player/minimap/Minimap.h"

#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"

#include "IkHumanoidModell/Ik/Controller/PlayerController/HumanoidPlayerController.h"

#include "playerScript.generated.h"

UCLASS()
class P2_API AplayerScript : public APlayerControllerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AplayerScript();


	void debugPathFinder();

	

	void reloadLoadout(LoadoutHelper &loadout);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//derived from ACharacter
	virtual void Jump() override;

	//tracks input for steering vehicles, and unseat
	InputContainer playerInputContainer;

	///overriden from base class!
	virtual bool IsPaused() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void updatePlayerEnteredAreaUi(bool entered);

private:
	bool isCamInPlayer = true;
	void switchCamera();
	FVector cameraReltiveLocationOriginal;

	void cameraDefaultFpv();
	void cameraDebugFpv();

	class playerInventory playerInventory;


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
	

	void keydown1();
	void keydown2();
	void keydown3();
	void keydown4();
	void switchToIndex(int index);



	//BONE CONTROLLER
	void TickBoneController(float DeltaTime);
	void resetFlagsOnTick();

	
	class HumanoidPlayerController boneController;
	// class BoneController boneController;
	AActor *createLimbPivotAtTop(int x, int y, int height, int pushFront);
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



	//Loadout setup
	void pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(
		Aweapon *weapon
	);


	//Minimap
	AMinimap *minimap = nullptr;
	void createMiniMap();
	void TickMiniMap();
};
