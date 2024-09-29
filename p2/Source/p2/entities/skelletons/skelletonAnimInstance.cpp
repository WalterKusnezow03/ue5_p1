// Fill out your copyright notice in the Description page of Project Settings.


#include "skelletonAnimInstance.h"


UskelletonAnimInstance::UskelletonAnimInstance(){

}


void UskelletonAnimInstance::SetLeftHandEffectorLocation(FVector newLocation){
    LeftHandEffectorLocation = newLocation;
}

void UskelletonAnimInstance::SetLeftElbowJointTargetLocation(FVector newLocation){
    LeftElbowJointTargetLocation = newLocation;
}

void UskelletonAnimInstance::SetRightHandEffectorLocation(FVector newLocation){
    RightHandEffectorLocation = newLocation;
}

void UskelletonAnimInstance::SetRightElbowJointTargetLocation(FVector newLocation){
    RightElbowJointTargetLocation = newLocation;
}