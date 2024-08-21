// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/entityManager/EntityManager.h"
#include "p2/util/FVectorUtil.h"
#include "particle.h"

// Sets default values
Aparticle::Aparticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aparticle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aparticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(enabledForTick){
		//apply movement
		move(DeltaTime);

		timer.Tick(DeltaTime);
		if(timer.timesUp()){
			//release self
			release();
		}
	}
	
}

void Aparticle::move(float DeltaTime){
	//x0
	FVector x_O = GetActorLocation();

	float scaleSlowedDown = (speed * DeltaTime - ((1 / 2) * (decrease * DeltaTime * DeltaTime)));
	if(scaleSlowedDown < 0){
		scaleSlowedDown = 0;
	}

	FVector testing = x_O + scaleSlowedDown * direction;
	SetActorLocation(testing);
	return;



	//m/s * s = m
	//v0 * t = meter
	//x(t) = x0 + v0t + (1/2)at^2
	//--> x(t) = x0 + v0t * vdir.normalasiert + 1/2at^2
	//FVector gravity = FVector(0, 0, (1 / 2 * decrease)) * (DeltaTime * DeltaTime);
	

	FVector scaled = x_O + (speed * DeltaTime) * direction;  // gx = A + r*(B - A)
	//scaled += gravity;
	SetActorLocation(scaled);
}

/// @brief apply an impulse to the particle, constant speed, no decelerate
/// @param directionIn direction, will be normalized
/// @param velocity constant velocity to apply 
void Aparticle::applyImpulse(FVector directionIn, float velocity, float lifeTime){
	direction = directionIn.GetSafeNormal();
	speed = velocity;

	//float lifeTime = 20.0f; //to be implemented randomly or function param
	timer.Begin(lifeTime);

	enabledForTick = true;
	show(true);
	disablePhysics();


	//apply random look rotation
	SetActorRotation(FVectorUtil::randomRotation(GetActorLocation()));

}

/// @brief disables physics entirely
void Aparticle::disablePhysics(){
	TArray<UStaticMeshComponent *> components;
	GetComponents<UStaticMeshComponent>(components);
	for (int i = 0; i < components.Num(); i++){
		UStaticMeshComponent *c = components[i];
		if(c != nullptr){
			c->SetSimulatePhysics(false);
		}
	}
}


/// @brief shows or hides the particle automatically
/// @param show 
void Aparticle::show(bool show){
	SetActorHiddenInGame(!show);  // Hides the actor if 'show' is false
}

/// @brief will release the particle to the entitymanager
void Aparticle::release(){
	show(false);
	enabledForTick = false;
	if(EntityManager *e = EntityManager::instance())
	{
		//to implement: release to entity manager for particles
	}
}