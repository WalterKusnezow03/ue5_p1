// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/BoneIk.h"
#include "Kismet/KismetMathLibrary.h"
#include "p2/entities/customIk/IkActor.h"

// Sets default values
AIkActor::AIkActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIkActor::BeginPlay()
{
	Super::BeginPlay();

	//init offset for now
	offset = FVector(1000, -1000, 200);
	ownLocation.setTranslation(offset);

	float legScaleCM = 200;
	leg1.setupBones(legScaleCM);
	leg2.setupBones(legScaleCM);

	float armScale = legScaleCM;
	arm1.setupBones(armScale);
	
	
	


	//works as expected

	/*
	arm1.setEthaFromCurrentRotation(0.5f); //0 being fully extended (?)
	arm1.rotateFirstLimbDeg(0, -90, 0);
	*/
	//arm1.rotateFirstLimbDeg(0, 0, 0);
	

	arm1.rotateFirstLimbDeg(0, -90, 0);
	
	//works as expected in default rotation / no rotation applied
	//testing needed with rotation applied, issues
	//FVector localTarget(1, 0, -1); //quasi grade aus hoch
	//arm1.rotateTowardsLocalTarget(localTarget); //analyse notwendig

	//FVector target2(0, -1.9f, 0);
	FVector target2(1.5f, -0.5f, 0); //sollte das selbe ergebnis prodzuieren bis jetzt: ja tut es.
	FVector weight(1, 1, -1);
	arm1.rotateEndToTarget(target2, weight); //testing notwendig
	

	targetA = FVector(1.5f, -0.5f, 0);
	targetB = FVector(1.5f, 1.0f, -1);
	targetA *= 100; //auf cm skalieren.
	targetB *= 100;

	//testing
	ownOrientation.yawRadAdd(MMatrix::degToRadian(90));

}


// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//new: rotation and location
	MMatrix updatedMatrix = ownLocation;
	


	updateBone(leg1, DeltaTime, FColor::Red);
	//updatePositionBasedOnMovedDistance(leg1);

	if(leg1.halfIsReached()){
		//updateBone(bone2, DeltaTime, FColor::Green);
	}

	

	//arms new, must be added
	FVector armOff = offset + FVector(0, 0, 100); // up offset for arms
	//arm1.build(GetWorld(), armOff, FColor::Purple, DeltaTime * 2);

	
	//neu braucht testing!
	MMatrix armMat = ownLocation * ownOrientation; //lesen rückwärts: 
	//rotation um eigene achse IM URSPRUNG
	//dann translation, aber rückwärts schreiben
	// R_T = T * R; //genau rückwärts rechnen

	//ownLocation;
	FVector verticalOffset(0, 0, 100);
	armMat += verticalOffset;
	arm1.build(GetWorld(), armMat, FColor::Purple, DeltaTime * 2);
	

	//debug draw of targets
	FVector t1 = targetA;
	FVector t2 = targetB;
	t1 += armOff;
	t2 += armOff;
	DebugHelper::showLineBetween(GetWorld(), t1, t2, FColor::Green);


	leg2.build(GetWorld(), armOff, FColor::Black, DeltaTime * 2);
	
	//draw forward line to approve arm is correct
	DebugHelper::showLineBetween(GetWorld(), armOff, armOff + FVector(100,0,0), FColor::Green, DeltaTime * 2);



	//arm pos follow testing
	debugDynamicArmTick(DeltaTime);

}



/// @brief updates a given bone
/// @param bone 
/// @param deltaTime 
/// @param color 
void AIkActor::updateBone(BoneIk &bone, float deltaTime, FColor color){
	//FVector offset(1000, -1000, 200);
	bone.tickMotion(GetWorld(), deltaTime, offset, color);
}


void AIkActor::updatePositionBasedOnMovedDistance(BoneIk &trackedBone){
	FVector pos = trackedBone.movedLastTick();
	float xMoved = pos.X; // say this is forward for now
	if(xMoved > 0){
		//if x < 0 means the leg moved the body forward
		offset.X += xMoved;

		ownLocation.setTranslation(offset);
	}
}



/// @brief look at a location
/// @param TargetLocation target to look at
void AIkActor::LookAt(FVector TargetLocation) 
{
    // Calculate the rotation needed to look at the target location
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	float zDegree = LookAtRotation.Yaw;

	ownOrientation.yawRad(MMatrix::degToRadian(zDegree));
}

// --- testing needed ---

void AIkActor::debugDynamicArmTick(float DeltaTime){

	//arm gets build in tick.

	float slow = 0.5f;
	timeCopy += DeltaTime * direction * slow;
	if(timeCopy < 0){
		direction *= -1;
		timeCopy = 0;
	}
	if(timeCopy > 1){
		direction *= -1;
		timeCopy = 1;
	}


	FVector directionVecAll = (targetB - targetA); //AB = B - A
	FVector posNew = targetA + directionVecAll * timeCopy;
	FVector weight(1, 1, -1);
	arm1.rotateEndToTarget(posNew, weight);


}