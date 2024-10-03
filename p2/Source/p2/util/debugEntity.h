// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/interfaces/Damageinterface.h"
#include "p2/player/teamEnum.h"
#include "p2/entities/customIk/BoneIk.h"

#include "debugEntity.generated.h"

UCLASS()
class P2_API AdebugEntity : public AActor, public IDamageinterface
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

	virtual void takedamage(int damage) override;
	virtual void takedamage(int damage, FVector &from) override;
	virtual void setTeam(teamEnum team) override;
	virtual teamEnum getTeam() override;

private:
	bool clicked;
	void debugFunction();

	void debugFunction(float deltaTime);
	float deg = 0.0f;
	float mpideg = 0.0f;

	class BoneIk bone;
	class BoneIk bone2;

	void spawnCustomIkActor();
};
