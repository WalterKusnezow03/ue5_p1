// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/team/teamEnum.h"
#include <map>
#include "Outpost.generated.h"

//forward declaration
class AHumanEntityScript;
class AOutpostAlarmPole;

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

	void createAlarmPolesIfNeeded();
	void init();

	void ClearAllOnEndPlay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ClearAlarmPoles();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	//distance
	static const int MAXDISTANCE_RADIUS = 5000; //50meters * 100 = 5000
	

	//will save for each team the entites. Needed for dynamic targets
	std::map<teamEnum, std::vector<AHumanEntityScript *>> teamMap;
	void removeFromMap(AHumanEntityScript *human);
	void addToMap(AHumanEntityScript *human);
	void removeFromVec(AHumanEntityScript *human, std::vector<AHumanEntityScript *> &vec);
	std::vector<AHumanEntityScript *>& getVectorReferenceFor(teamEnum team);



	//entities
	void createEntity(teamEnum t);
	void createEntity(int count, teamEnum t);

	void moveAllEntitiesToGroundOnPlayerEnterArea();

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
	FVector randomOffsetFromActorLocation(int range);

	bool playerEntered = false;
	void switchPlayerEnteredStatus(bool status);

public:
	//alert / alarm all entites
	void alertAll();
	void alarmAll();

	void UpdatePlayerEnteredUi();

	//team leader section
	void validatePlayerTargetMovingPosition(
		AHumanEntityScript *botPointer,
		FVector &position
	);

private:
	std::map<teamEnum, AHumanEntityScript *> teamLeaders;
	bool isTeamLeader(AHumanEntityScript *bot);
	void replaceTeamLeaderIfNeededOnRemoveOf(AHumanEntityScript *botPointer);
	void pickTeamLeaderIfNeeded(teamEnum team);
	bool newTeamLeaderNeeded(teamEnum team);




	std::vector<AOutpostAlarmPole *> alarmPoles;
	void resetAlarmPoles();
};
