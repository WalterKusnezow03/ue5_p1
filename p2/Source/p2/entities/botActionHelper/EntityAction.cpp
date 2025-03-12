// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityAction.h"
#include "EActionType.h"

EntityAction::EntityAction(){
    typeOfAction = EActionType::ERoam;
	targetPositionIsSetup = false;
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