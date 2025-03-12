// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EActionType.h"

/**
 * 
 */
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

private:
	EActionType typeOfAction = EActionType::ERoam;

	FVector targetPositionSaved;
	bool targetPositionIsSetup = false;

};
