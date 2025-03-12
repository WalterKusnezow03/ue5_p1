// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionManager.h"

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



bool ActionManager::hasHigherPriorityThanCurrentAction(EActionType type){
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
    currentActionTask = type;
}

void ActionManager::changeToActionIfPossible(EActionType type){
    if(hasHigherPriorityThanCurrentAction(type)){
        currentActionTask = type;
    }
    
}
void ActionManager::changeToActionIfPossible(EActionType type, FVector &target){
    if(hasHigherPriorityThanCurrentAction(type)){
        currentActionTask = type;
        setupAction(type, target); //gets overriden
    }
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
    if(actionMap.find(currentActionTask) == actionMap.end()){
        createDefaultAction(currentActionTask);
    }
    
    return actionMap[currentActionTask];
}

