// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityAction.h"
#include "EActionType.h"

EntityAction::EntityAction(){
    typeOfAction = EActionType::ERoam;
	targetPositionIsSetup = false;
    hasTimerRunning = false;
}

EntityAction::EntityAction(EActionType typeOfActionIn)
{
    typeOfAction = typeOfActionIn;
    targetPositionIsSetup = false;
}

EntityAction::EntityAction(EActionType typeOfActionIn, FVector &target){
    typeOfAction = typeOfActionIn;
    updateTargetPosition(target);
}

EntityAction::EntityAction(const EntityAction &other){
    *this = other;
}

EntityAction &EntityAction::operator=(const EntityAction & other){
    if(this == &other){
        return *this;
    }
    typeOfAction = other.typeOfAction;
    targetPositionSaved = other.targetPositionSaved;
	targetPositionIsSetup = other.targetPositionIsSetup;


    return *this;
}

EntityAction::~EntityAction()
{

}


bool EntityAction::hasTargetPosition(){
    return targetPositionIsSetup;
}

FVector EntityAction::targetPosition(){
    return targetPositionSaved;
}

void EntityAction::updateTargetPosition(FVector &pos){
    targetPositionIsSetup = true;
    targetPositionSaved = pos;
}

EActionType EntityAction::actionType(){
    return typeOfAction;
}



bool EntityAction::Tick(float deltatime){
    if(hasTimerRunning){
        timer.Tick(deltatime);

	    if(timer.timesUp()){
            hasTimerRunning = false;
            return true;
        }
    }
    return false; //switch not needed
}


EActionType EntityAction::GetFollowingActionType(){
    return typeOfActionAfterTimeFinish;
}


void EntityAction::StartTimer(float time, float delayTime){
    if(time >= 0.0f){
        hasTimerRunning = true;
        timer.Begin(time);
        timeInitial = time;
        delay = delayTime;
    }
}

void EntityAction::StartTimer(float time, EActionType nexttype, float delayTime){
    if(time >= 0.0f){
        StartTimer(time, delayTime);
        typeOfActionAfterTimeFinish = nexttype;
    }
}


bool EntityAction::HasTimer(){
    return hasTimerRunning;
}

bool EntityAction::TimerDelayPassed(){
    if(hasTimerRunning){
        if(timeInitial >= 0.0f){
            float timePassed = timeInitial - timer.currentTimeLeft(); //AB = B - A
            return timePassed > delay;
        }
    }
    return true;
}

void EntityAction::abortTimer(){
    hasTimerRunning = false;
}



//event based
void EntityAction::StartWithEvent(FString eventName, EActionType nexttype){
    hasEventStringAttached = true;
    attachedEventString = eventName;
    typeOfActionAfterTimeFinish = nexttype;
}

bool EntityAction::TryCompleteWithEvent(FString name){
    if(MatchesEventName(name)){
        hasEventStringAttached = false;
        abortTimer();
        return true;
    }
    return false;
}

bool EntityAction::MatchesEventName(FString eventName){
    if(hasEventStringAttached){
        return eventName.Equals(attachedEventString);
    }
    return false;
}