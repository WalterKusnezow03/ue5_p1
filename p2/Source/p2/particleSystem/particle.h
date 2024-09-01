// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/util/timer.h"
#include "particleEnum.h"
#include "particle.generated.h"

UCLASS()
class P2_API Aparticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aparticle();
	void setParticleType(particleEnum typeIn);
	particleEnum getType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void disablePhysics();
	void disableCollider();
	void move(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void applyImpulse(FVector directionIn, float speedIn, float lifeTime);

private:
	FVector direction;
	float speed;
	float decrease = -9.81f;
	class timer timer;

	enum class particleEnum type;

	bool enabledForTick;

	void release();
	void show(bool show);
};
