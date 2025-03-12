
#pragma once

#include "CoreMinimal.h"
#include "p2/entities/botActionHelper/EActionType.h"
#include "p2/entities/botActionHelper/EntityAction.h"


/**
 * 
 */
class P2_API ActionManager
{
public:
	ActionManager();
	~ActionManager();

	void setupAction(EActionType type);
	void setupAction(EActionType type, FVector &targetPosition);

	EntityAction &currentAction();

	void changeToAction(EActionType type);
	void changeToActionIfPossible(EActionType type);
	void changeToActionIfPossible(EActionType type, FVector &target);

private:
	void createDefaultAction(EActionType type);
	bool hasHigherPriorityThanCurrentAction(EActionType type);

	std::map<EActionType, EntityAction> actionMap;
	EActionType currentActionTask = EActionType::ERoam;

	void setupDefaultTasks();
};
