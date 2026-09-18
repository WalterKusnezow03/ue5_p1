// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EActionType.h"
#include "CoreMath/animation/timer/Timer.h"

///Entity action to be used from actionamanger and entities
///to switch between different tasks
class P2_API EntityAction
{
public:
	EntityAction();
	EntityAction(EActionType typeOfActionIn);
	EntityAction(EActionType typeOfActionIn, FVector &target);
	EntityAction(const EntityAction &other);
	EntityAction &operator=(const EntityAction &other);

	~EntityAction();

	EActionType actionType();
	bool hasTargetPosition();
	void updateTargetPosition(FVector &pos);
	FVector targetPosition();

	//if times up, switch to next action
	bool Tick(float deltatime);
	EActionType GetFollowingActionType();
	void StartTimer(float time, EActionType nexttype);
	bool HasTimer();
	void abortTimer();

private:
	EActionType typeOfAction = EActionType::ERoam;

	FVector targetPositionSaved;
	bool targetPositionIsSetup = false;

	/// --- timed switch of action ---
	EActionType typeOfActionAfterTimeFinish = EActionType::ENone;
	Timer timer;
	bool hasTimerRunning = false;


};
