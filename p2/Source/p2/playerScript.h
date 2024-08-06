// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "weapon.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent


#include "Damageinterface.h"

#include "playerScript.generated.h"

UCLASS()
class P2_API AplayerScript : public ACharacter, public IDamageinterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AplayerScript();
	virtual void takedamage(int d) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere)
    class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
    class UAnimInstance* AnimInstance;

private:
	bool aiming;
	bool holding;

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

	class Aweapon *weaponpointer;

	//UPROPERTY(VisibleAnywhere)
    //class UCameraComponent* CameraComponent;
	//class
};
