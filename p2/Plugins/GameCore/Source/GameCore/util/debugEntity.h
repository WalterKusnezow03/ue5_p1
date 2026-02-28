// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"
#include "GameCore/team/teamEnum.h"


#include "debugEntity.generated.h"

UCLASS()
class GAMECORE_API AdebugEntity : public AActor, public IDamageinterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AdebugEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void takedamage(FCustomHitResult &result) override;

	virtual void setTeam(teamEnum team) override;
	virtual teamEnum getTeam() override;

	virtual bool IsInRange(const FVector &position, float maxDistance) override;

private:
	bool clicked;
	void debugFunction();

	void debugFunction(float deltaTime);
	float deg = 0.0f;
	float mpideg = 0.0f;


};
