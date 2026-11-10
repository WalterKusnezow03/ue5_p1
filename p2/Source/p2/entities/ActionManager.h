
#pragma once

#include "CoreMinimal.h"
#include "p2/entities/botActionHelper/EActionType.h"
#include "p2/entities/botActionHelper/EntityAction.h"


///Entity action manager to switch between states
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

	//timed actions
	void changeToActionTimed(EActionType wantedType, float time);
	void changeToActionTimed(EActionType wantedType, float time, EActionType nextAction);

	//event wait actions
	void changeToActionWaitForEvent(EActionType wantedType, FString waitForEvent);

	//event wait actions with auto kill timer
	void changeToActionWaitForEvent(
		EActionType wantedType, 
		FString waitForEvent, 
		float timeUntilAbort,
		float delayBeforeAbort = 0.0f
	);

	//cancels the timer if any setup, if so and is currenttask, switch to targeted next task
	void abortTimerOnAction(EActionType type);
	bool abortTimerOnEvent(FString event);

	//check if action matches a name
	bool ActionMatchesEventName(FString event);
	

	//some actions might be timed and cancelled after the timer finishes
	void Tick(float deltatime);
	

private:
	void createDefaultAction(EActionType type);
	bool hasHigherPriorityThanCurrentAction(EActionType type);

	EntityAction &FindAction(EActionType type);

	std::map<EActionType, EntityAction> actionMap;
	EActionType currentActionTask = EActionType::ERoam;

	void setupDefaultTasks();
};
