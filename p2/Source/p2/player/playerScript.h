// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "weapon.h"
#include "playerInventory.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent

#include "teamEnum.h"
#include "p2/interfaces/Damageinterface.h"
#include "p2/entities/customIk/bonePackage/BoneController.h"
#include "p2/util/timer.h"

#include "playerScript.generated.h"

UCLASS()
class P2_API AplayerScript : public ACharacter, public IDamageinterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AplayerScript();
	virtual void takedamage(int d) override;
	virtual void takedamage(int d, FVector &hitpoint) override;
	virtual void setTeam(teamEnum teamIn) override;
	virtual teamEnum getTeam() override;

	void debugPathFinder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//derived from ACharacter
	virtual void Jump() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere)
    class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponentSecondary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
    class UAnimInstance* AnimInstance;

private:
	bool isCamInPlayer = true;
	void switchCamera();
	FVector cameraReltiveLocationOriginal;

	void cameraDefaultFpv();
	void cameraDebugFpv();

	teamEnum team;

	class playerInventory playerInventory;


	// Pointer to the Skeletal Mesh Component
    class USkeletalMeshComponent* SkeletalMeshComponent;

	bool aiming;
	bool holding;
	bool sprinting;
	void sprint();
	static const int SPRINT_MULTIPLY = 3;

	bool isWalking;


	void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);
	void interact();
	void reload();
	void drop();
	void aim();

	void setHolding(bool h);
	void shoot();
	void leftMouseUp();
	void leftMouseDown();

	void performRaycast();

	UPROPERTY(EditAnywhere)
    float TurnRateGamepad;

    UPROPERTY(EditAnywhere)
    float LookUpRateGamepad;

	int health;

	

	



	//new
	void TickBoneController(float DeltaTime);
	void resetFlagsOnTick();

	class BoneController boneController;
	void setupBoneController();
	AActor *createLimbPivotAtTop(int x, int y, int height, int pushFront);

	//new wingsuit
	bool wingsuitTimerWasStarted = false;
	bool wingsuitIsOpen = false;
	float wingsuitUpdateInvertall = 2.0f;
	class timer wingsuitTimer;
	int minDistanceGroundForWingsuit = 200; //10000
	void setWingsuitTimerOnMovement();
	void setWingsuitTimer(float time);
	void TickWingsuitTimer(float DeltaTime);
	void tryOpenWingsuit();
	bool isInAirRaycast(FVector Start, float &distanceMeasured);
	void changeGravityDefault();
	void changeGravityWingSuit();

	float gravityCmsDown();
};
