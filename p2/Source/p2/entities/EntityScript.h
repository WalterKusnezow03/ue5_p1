// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/interfaces/Damageinterface.h"
#include "p2/player/playerScript.h"
#include "p2/player/teamEnum.h"
#include "p2/util/timer.h"

#include "EntityScript.generated.h"

UCLASS()
class P2_API AEntityScript : public AActor, public IDamageinterface
{
	GENERATED_BODY()
	
public:
	

	// Sets default values for this actor's properties
	AEntityScript();
	//inherited from interface
	virtual void takedamage(int d) override;
	virtual void takedamage(int d, FVector &from) override;

	bool isActivatedForUpdate(); //VERY IMPORTANT DO NOT REMOVE

	//max distance to follow
	static const int MAXDISTANCE = 5000; //50m --> 50 * 100 = 5000

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void init();

	//virtual void setTeam(int teamIn) override;
	//virtual int getTeam() override;
	
	virtual void setTeam(teamEnum teamIn) override;
	virtual teamEnum getTeam() override;

	virtual bool isWithinMaxRange(FVector vec);

	//must be public
	void enableActiveStatus(bool enable);
	//activate methods for manager end

	virtual void despawn();

	void alert();
	void alert(FVector lookat);
	void alarm(); //sets spotting status to true

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//int team;
	teamEnum team;

	// performs the raycast to a target
	bool performRaycast(AActor *target);
	bool performRaycast(FVector &direction, FVector &output, int cmLength);

	//raycast ignore params (setup on begin)
	FCollisionQueryParams ignoreParams;
	//CALL ONCE ON BEGIN!
	void setupRaycastIgnoreParams();

	void setSpottingTime(float time);
	void updateSpottingTime(float deltaTime);

	void LookAt(FVector TargetLocation); 
	void LookAt(AActor *target);
	bool withinVisionAngle(AActor *target);

	//path section
	std::vector<FVector> path;
	void moveTowardsPlayer(float deltaTime);
	void resetpath();
	void followpath(float deltaTime);
	bool hasNodesInPathLeft();
	bool reachedPosition(FVector pos);

	/// @brief saves the time left for delay, if no path is found the timer should be reset
	/// allows the player to move and better path to be found, saves performance
	float pathDelay;
	/// @brief reset the path delay time to a number of seconds
	/// @param time time in deconds
	void resetPathDelay(float time);
	/// @brief update the path delay time, called from tick
	/// @param DeltaTime deltatime since last frame
	void updatePathDelay(float DeltaTime);
	///returns if any time is left to be waited before asking for a new path
	bool pathDelayRunning();
	//path section end


	int health;
	float defaultSpottingTime;
	float spottingTimeLeft;

	bool spottedPlayer;
	bool canSeePlayer;

	class AplayerScript* playerPointer;

	

	void showScreenMessage(FString s);


	virtual void die();
	bool activated;
	
	void enableCollider(bool enable);
	
	class AskelletonController *skelletonControllerPointer = nullptr;




	//timer for grounding
	class timer groundProjectionTimer;
};
