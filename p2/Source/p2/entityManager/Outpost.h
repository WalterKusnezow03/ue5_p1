// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/player/teamEnum.h"
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
	//distance
	static const int MAXDISTANCE_RADIUS = 10000; //50meters * 100 = 5000
	static const int MAXDISTANCE_METERS = 100;

	//will save for each team the entites. Needed for dynamic targets
	std::map<teamEnum, std::vector<AHumanEntityScript *>> teamMap;
	void removeFromMap(AHumanEntityScript *human);
	void addToMap(AHumanEntityScript *human);
	void removeFromVec(AHumanEntityScript *human, std::vector<AHumanEntityScript *> &vec);
	std::vector<AHumanEntityScript *>& getVectorReferenceFor(teamEnum team);



	//entities
	std::vector<AHumanEntityScript *> myEntities;
	void createEntity(teamEnum t);
	void createEntity(int count, teamEnum t);

	

	//alarm
	bool alertEnabled;
	bool alarmEnabled;

	//liberate
	bool isLiberated;
	void tryliberate();

	//despawn all
	void releaseAll();

	void initEntitiesIfNeeded();

	FVector randomOffset(int range);


	bool playerEntered = false;
	void switchPlayerEnteredStatus(bool status);

public:
	//alert / alarm all entites
	void alertAll();
	void alarmAll();
};
