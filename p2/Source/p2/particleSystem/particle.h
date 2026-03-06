// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreMath/animation/timer/Timer.h"
#include "AssetEnumCollection/assetEnums/particleEnum.h"
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

	void applyImpulse(FVector &directionIn, float speedIn, float lifeTime);
	void applyImpulse(FVector &directionIn, float velocity, float lifeTime, UMaterialInterface *material);
	void applyImpulse(
		FVector &directionIn,
		float velocity,
		float lifeTime,
		FVector &scale
	);

	void applyImpulse(
		FVector &directionIn,
		float velocity,
		float lifeTime,
		UMaterialInterface *material,
		FVector &scale
	);

private:
	FVector direction;
	float speed;
	float decrease = -9.81f;
	Timer timer;

	enum class particleEnum type;

	bool enabledForTick;

	void release();
	void show(bool show);

	void findStaticMesh();
	UStaticMeshComponent *staticMeshComponent = nullptr;
};
