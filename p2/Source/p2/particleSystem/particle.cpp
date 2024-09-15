// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/entityManager/EntityManager.h"
#include "p2/util/FVectorUtil.h"
#include "p2/particleSystem/particleEnum.h"
#include "particle.h"

// Sets default values
Aparticle::Aparticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	type = particleEnum::particleNone_enum; //default
}

// Called when the game starts or when spawned
void Aparticle::BeginPlay()
{
	Super::BeginPlay();
	findStaticMesh();
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

/// @brief will move the particle from tick
/// @param DeltaTime 
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
	
}


/// @brief apply an impulse to the particle, constant speed, no decelerate
/// @param directionIn direction, will be normalized
/// @param velocity constant velocity to apply 
/// @param lifeTime lifetime in seconds before releasing again
/// @param material material to set if possible
void Aparticle::applyImpulse(FVector &directionIn, float velocity, float lifeTime, UMaterial *material){
	//apply random scale
	FVector newScale = FVectorUtil::randomScale(1, 3, true);
	applyImpulse(directionIn, velocity, lifeTime, material, newScale);
}

/// @brief apply an impulse to the particle, constant speed, no decelerate
/// @param directionIn direction, will be normalized
/// @param velocity constant velocity to apply 
/// @param lifeTime lifetime in seconds before releasing again
void Aparticle::applyImpulse(FVector &directionIn, float velocity, float lifeTime){
	//apply random scale
	FVector newScale = FVectorUtil::randomScale(1, 3, true);
	applyImpulse(directionIn, velocity, lifeTime, nullptr, newScale);
}



void Aparticle::applyImpulse(
	FVector &directionIn, 
	float velocity, 
	float lifeTime,
	FVector &scale
){
	applyImpulse(directionIn, velocity, lifeTime, nullptr, scale);
}




void Aparticle::applyImpulse(
	FVector &directionIn, 
	float velocity, 
	float lifeTime, 
	UMaterial *material,
	FVector &scale
){
	if(material != nullptr){
		findStaticMesh();
		if(staticMeshComponent != nullptr){
			staticMeshComponent->SetMaterial(0, material); //index 0
		}
	}


	direction = directionIn.GetSafeNormal();
	speed = velocity;

	//float lifeTime = 20.0f; //to be implemented randomly or function param
	timer.Begin(lifeTime);

	enabledForTick = true;
	show(true);
	disablePhysics();
	disableCollider();

	//apply random look rotation
	SetActorRotation(FVectorUtil::randomRotation(GetActorLocation()));

	//apply random scale
	scale = FVectorUtil::abs(scale);
	SetActorScale3D(scale);
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

void Aparticle::disableCollider(){
	SetActorEnableCollision(false);
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
	if (EntityManager *e = worldLevel::entityManager())
	{
		//to implement: release to entity manager for particles
		e->add(this);
	}
}

/// @brief sets the particle type the entity manager should take back once released
/// @param typeIn type the particle to be on paper
void Aparticle::setParticleType(particleEnum typeIn){
	this->type = typeIn;
}


particleEnum Aparticle::getType(){
	return type;
}


/// @brief finds the static mesh component from aactor (if not found yet)
void Aparticle::findStaticMesh(){
	if(staticMeshComponent == nullptr){
		std::vector<UStaticMeshComponent*> out;
		AActorUtil::findAllComponentsByType(*this, out);

		for (int i = 0; i < out.size(); i++){
			UStaticMeshComponent *s = out.at(i);
			if(s != nullptr){
				staticMeshComponent = s;
				return;
			}
		}
	}
}