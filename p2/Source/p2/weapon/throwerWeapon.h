// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weapon.h"
#include "p2/throwableItems/throwableItem.h"
#include "throwerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class P2_API AthrowerWeapon : public Aweapon
{
	GENERATED_BODY()

public:
	AthrowerWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void showWeapon(bool show) override;

	void setThrowableType(throwableEnum type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "throwabletype")
	throwableEnum throwableTypeToThrow; //type to throw / instantiate

protected:
	virtual void shootProtected(FVector from, FVector to, int ownTeam) override;
	virtual void reload(int amount) override;

	bool showThrowableItem;
	AthrowableItem *throwableActorPointer;

	void requestNewThrowable();


	
};
