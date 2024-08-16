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

	//distance
	static const int MAXDISTANCE = 200000; //200m

	//alarm
	bool alertEnabled;
	bool alarmEnabled;


	void liberate();

public:
	void alertAll();
	void alarmAll();
};
