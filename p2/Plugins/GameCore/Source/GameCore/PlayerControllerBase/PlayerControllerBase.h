// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "GameCore/team/teamEnum.h"
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"
#include "CoreMath/animation/timer/timer.h"
#include "GameCore/Input/InputContainer.h"
#include "GameCore/interfaces/Steeringinterface.h"

#include "customuipluginbase/UserInputTracking/UserInput.h"

#include "PlayerControllerBase.generated.h"

UCLASS()
class GAMECORE_API APlayerControllerBase : public ACharacter, public IDamageinterface, public ISteeringinterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerControllerBase();
	virtual void takedamage(FCustomHitResult &result) override;
	virtual void setTeam(teamEnum teamIn) override;
	virtual teamEnum getTeam() override;

	virtual bool IsInRange(const FVector &position, float maxDistance) override;

	void showCursor(bool show);
	void setPaused(bool in);
	void EjectMouse();

	void SetMovementLocked(bool flag);

	//steering
	virtual InputContainer &input() override;
	virtual void setDriverLocation(FVector &location) override;

	//returns the player controller of this character
	APlayerController *GetPlayerController();

	void CollectUserInput(UserInput &input);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//derived from ACharacter
	virtual void Jump() override;

	//tracks input for steering vehicles, and unseat
	InputContainer playerInputContainer;

	/// @brief should be overriden!
	virtual bool IsPaused();

	bool MovementMarkedLocked();

public:	
	// Called every frame
	///@brief processPendingRecoil called,
	/// PlayerInfo::Update called
	/// UpdateCursorVisibilityBasedOnPause
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere)
    class UCameraComponent* CameraComponent;

	//UPROPERTY(VisibleAnywhere)
	//class UCameraComponent* CameraComponentSecondary;

	/// --- camera helpers ---
	FRotator cameraRotation();
	FVector playerLookDir();
	FVector CameraLocation();

protected:
    //LATEST INTERACTED ACTOR POINTER
    AActor *interactedActorPointer = nullptr;

    teamEnum team;

	bool aiming;
	bool holding;
	bool sprinting;
	void sprint();
	static const int BASE_SPEED = 600;
	static const int SPRINT_SPEED = 700;
    float playerMaxMovementVelocity = 500.0f;

	bool isWalking;

    AActor *performRaycast();

    
	// ---- KEYBOARD ----

	virtual void MoveForward(float Value);
    virtual void MoveRight(float Value);
    
	virtual void interact();
	virtual void reload();
	virtual void drop();
	virtual void aim();

	
	virtual void openPauseMenu();
    

	// ---- MOUSE INTERACTION ----
	void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    virtual void leftMouseUp();
    virtual void leftMouseDown();
	void setHolding(bool h);

	

    UPROPERTY(EditAnywhere)
    float TurnRateGamepad;

    UPROPERTY(EditAnywhere)
    float LookUpRateGamepad;

	int health;


	

	void processPendingRecoil();
	void addPendingRecoil(float value);
	float pendingRecoil = 0.0f;


private:
	//fall back flag! 
	bool isPausedFlag = false;

	bool isMovementLocked = false;

	void UpdateCursorVisibilityBasedOnPause();
	bool cursorVisibleFlag = false;
};
