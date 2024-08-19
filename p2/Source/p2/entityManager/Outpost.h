// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Outpost.generated.h"

//forward declaration
class AHumanEntityScript;

UCLASS()
class P2_API AOutpost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOutpost();

	void releaseEntity(AHumanEntityScript *entity);
	void subscribe(AHumanEntityScript *entity);
	bool playerInArea();

	bool isInRange(FVector &vec);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	//entities
	std::vector<AHumanEntityScript *> myEntities;
	void createEntity();
	void createEntity(int count);

	//distance
	static const int MAXDISTANCE = 70000; //70m
	static const int MAXDISTANCE_METERS = 70;

	//alarm
	bool alertEnabled;
	bool alarmEnabled;

	//liberate
	bool isLiberated;
	void liberate();

	//despawn all
	void releaseAll();

	void initEntitiesIfNeeded();

	FVector randomOffset(int range);

public:
	//alert / alarm all entites
	void alertAll();
	void alarmAll();
};
