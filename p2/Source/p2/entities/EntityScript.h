// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"
#include "GameCore/PlayerControllerBase/InteractionCallbackInterface/WidgetInteractionCallbackInterface.h"
#include "p2/entities/widgetPayloads/WidgetEntityLootPayload.h"

#include "p2/player/playerScript.h"
#include "GameCore/team/teamEnum.h"
#include "CoreMath/animation/timer/timer.h"

#include "ActionManager.h"

#include "IkHumanoidModell/Ik/Controller/BaseController/HumanoidController.h"

#include "p2/entetiesBase/miniMapRegisteredActor/MiniMapRegisteredActor.h"

#include "EntityScript.generated.h"

class AInteractWidgetActor;

UCLASS()
class P2_API AEntityScript : 
public AMiniMapRegisteredActor, 
public IDamageinterface, 
public IWidgetInteractionCallbackInterface //callback on E pressed to release widget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flags")
	bool debugStaticEntity = false;





	const bool DRAW_PATH = true;

	// Sets default values for this actor's properties
	AEntityScript();
	//inherited from interface
	virtual void takedamage(FCustomHitResult &result) override;
	virtual bool IsInRange(const FVector &position, float maxDistance) override;

	bool isActivatedForUpdate(); //VERY IMPORTANT DO NOT REMOVE

	//max distance to follow
	static const int MAXDISTANCE = 10000; //100m --> 100 * 100 = 5000
	static const int MAXDISTANCE_CLOSERANGE = 5000; //50m

	//may be deprecated, happens in humanoid controller
	void projectActorToGround();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//called on begin play and get from gc entity manager
	virtual void init();
	virtual void init(FVector &location);

	//virtual void setTeam(int teamIn) override;
	//virtual int getTeam() override;
	
	virtual void setTeam(teamEnum teamIn) override;
	virtual teamEnum getTeam() override;

	// --- mini map marker type derived ----
	EMarkerType GetMarkerType() override;

	virtual bool isWithinMaxRange(FVector vec);
	bool isWithinCloseRange(FVector &vec);

	//must be public
	void enableActiveStatus(bool enable);
	//activate methods for manager end

	virtual void despawn();

	void alert();
	virtual void alert(FVector lookat);
	void alarm(); //sets spotting status to true

protected:
	void FindPlayerReferenceIfNeeded();
	void UpdateCanSeePlayerStatus();
	void UpdateVisionTimers(float DeltaTime);

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
	virtual void requestNewPathTo(FVector &location, bool towardsPlayer);

	void resetpath();
	void followpath(float deltaTime);
	bool hasNodesInPathLeft();
	bool reachedPosition(FVector pos);

	void updateSpottingTimeOnAlert();
	void updateToReducedSpottingTimeIfNotSpottedYet();

	/// @brief reset the path delay time to a number of seconds
	/// @param time time in deconds
	void resetPathDelay(float time);
	/// @brief update the path delay time, called from tick
	/// @param DeltaTime deltatime since last frame
	void updatePathDelay(float DeltaTime);
	///returns if any time is left to be waited before asking for a new path
	bool pathDelayRunning();
	//path section end

	void drawPath();

	int health;
	float defaultSpottingTime;
	float spottingTime;

	bool spottedPlayer;
	bool canSeePlayer;

	class AplayerScript* playerPointer;

	


	virtual void die();
	bool activated;
	
	void enableCollider(bool enable);



	int pathDelayTime = 10;
	class Timer pathDelayTimer;

	class Timer spottingTimer;

	//TODO!
	class Timer despawnBodyTimer;

	//NEW ACTION MANAGER
	void actUponCurrentAction(float DeltaTime);
	ActionManager actionManager;






	//--humanoid controller--
	class Timer debugRecordRotationTimer;
	HumanoidController humanoidPluginController;

	void BeginPlayHumanoidController();
	void TickHumanoidController(float deltatime);
	void CopyHumanoidControllerTransform();



	//-- interaction widget on death --
public:
	virtual void ReceiveCallback() override;

protected:
	//will also call the setup loot payload.
	void RequestInteractWidget();
	void ReleaseInteractWidget();
	AInteractWidgetActor *currentInteractWidget = nullptr;

	UPROPERTY()
	UWidgetEntityLootPayload *deathLootPayload = nullptr;
	
	void SetupLootDeathPayloadAndRequestInteractWidget();
	virtual void SetupLootDeathPayload();
	void CreatePayloadIfNeeded();
};
