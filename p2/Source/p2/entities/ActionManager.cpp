// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionManager.h"
#include "DebugPlugin/DebugHelper.h"

ActionManager::ActionManager()
{
    setupDefaultTasks();
}

ActionManager::~ActionManager()
{
}


void ActionManager::setupDefaultTasks(){
    currentActionTask = EActionType::ERoam;
    createDefaultAction(EActionType::ERoam);
    createDefaultAction(EActionType::EMoveToPlayer);
}

void ActionManager::createDefaultAction(EActionType type){
    EntityAction moveAction(type);
    actionMap[type] = moveAction;   
}

void ActionManager::Tick(float deltatime){
    EntityAction &action = currentAction();
    if(action.Tick(deltatime)){ //timer finished
        EActionType switchTo = action.GetFollowingActionType();
        changeToAction(switchTo);
    }
}


bool ActionManager::hasHigherPriorityThanCurrentAction(EActionType type){
    if(currentAction().HasTimer()){
        return false;
    }

    if(currentActionTask == EActionType::ERoam){
        return true;
    }
    if(currentActionTask == EActionType::EMoveToSpecialPosition){
        return false;
    }
    return true;
}

/// PUBLIC API

void ActionManager::changeToAction(EActionType type){
    if(type == EActionType::ENone){
        type == EActionType::ERoam;
    }

    currentActionTask = type;
}

void ActionManager::changeToActionIfPossible(EActionType type){
    if(hasHigherPriorityThanCurrentAction(type)){
        changeToAction(type);
    }
}
void ActionManager::changeToActionIfPossible(EActionType type, FVector &target){
    if(hasHigherPriorityThanCurrentAction(type)){
        changeToAction(type);
        setupAction(type, target); //gets overriden
    }
}


void ActionManager::changeToActionTimed(EActionType wantedType, float time){
    changeToActionTimed(wantedType, time, currentActionTask);
}

void ActionManager::changeToActionTimed(EActionType wantedType, float time, EActionType nextAction){
    changeToAction(wantedType);
    EntityAction &action = FindAction(wantedType);
    action.StartTimer(time, nextAction);
}

void ActionManager::changeToActionWaitForEvent(EActionType wantedType, FString waitForEvent){
    EActionType currentCopy = currentActionTask;

    changeToAction(wantedType);
    EntityAction &action = FindAction(wantedType);
    action.StartWithEvent(waitForEvent, currentCopy);
}

void ActionManager::changeToActionWaitForEvent(
    EActionType wantedType, 
    FString waitForEvent, 
    float timeUntilAbort,
    float delayBeforeAbort
){
    changeToActionWaitForEvent(wantedType, waitForEvent);

    //or wait for timer
    EntityAction &action = FindAction(wantedType);
    action.StartTimer(timeUntilAbort, delayBeforeAbort);
}

void ActionManager::abortTimerOnAction(EActionType type){
    EntityAction &actionToAbort = FindAction(type);
    if(actionToAbort.HasTimer()){
        changeToAction(actionToAbort.GetFollowingActionType());
        actionToAbort.abortTimer();
    }
}

bool ActionManager::abortTimerOnEvent(FString event){
    EntityAction &actionToAbort = currentAction();
    if(
        actionToAbort.TimerDelayPassed() && 
        actionToAbort.TryCompleteWithEvent(event)
    ){
        DebugHelper::logMessage("ActionManager::abortTimerOnEvent with ", event);
        changeToAction(actionToAbort.GetFollowingActionType());
        return true;
    }
    return false;
}

bool ActionManager::ActionMatchesEventName(FString event){
    EntityAction &action = currentAction();
    return action.MatchesEventName(event);
}

///@brief will OVERRIDE the given action of enum type!
void ActionManager::setupAction(EActionType type){
    EntityAction action(type);
    actionMap[action.actionType()] = action;
}

///@brief will OVERRIDE the given action of enum type!
void ActionManager::setupAction(EActionType type, FVector &targetPosition){
    EntityAction action(type, targetPosition);
    actionMap[action.actionType()] = action;
}


EntityAction &ActionManager::currentAction(){
    /*if(actionMap.find(currentActionTask) == actionMap.end()){
        createDefaultAction(currentActionTask);
    }
    return actionMap[currentActionTask];*/
    return FindAction(currentActionTask);
}

EntityAction &ActionManager::FindAction(EActionType type){
    if(actionMap.find(type) == actionMap.end()){
        createDefaultAction(type);
    }
    return actionMap[type];
}
