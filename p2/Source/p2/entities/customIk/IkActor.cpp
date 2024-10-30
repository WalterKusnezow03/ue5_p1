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

	float armScale = legScaleCM * 0.8f;
	arm1.setupBones(armScale);
	
	
	


	//works as expected

	/*
	arm1.setEthaFromCurrentRotation(0.5f); //0 being fully extended (?)
	arm1.rotateFirstLimbDeg(0, -90, 0);
	*/
	//arm1.rotateFirstLimbDeg(0, 0, 0);
	

	//arm1.rotateFirstLimbDeg(0, -90, 0);
	
	//FVector target2(0, -1.9f, 0);

	//x ist quasi forward
	FVector target2(1.5f, -0.5f, 0); //sollte das selbe ergebnis prodzuieren bis jetzt: ja tut es.
	FVector weight(1, 1, -1);
	arm1.rotateEndToTarget(target2, weight); //testing notwendig
	

	targetA = FVector(1.0f, -0.5f, 0);
	targetB = FVector(1.5f, 1.0f, -1);
	targetA *= 100; //auf cm skalieren.
	targetB *= 100;

	//testing
	ownOrientation.yawRadAdd(MMatrix::degToRadian(90)); 
	//unklar wie sich dann bewegung verhält, ob x forward bleibt
	//oder ob man auf x und y die distanz messen muss, zumal sich der rote fuss auch nicht mitdreht!

}


// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//new: rotation and location

	//rotation und transformation muss quasi einheitlich applied werden auf 
	//alle vorhandenen knochen 


	//updateBone(leg1, DeltaTime, FColor::Red);
	updatePositionBasedOnMovedDistance(leg2); //movement for own location

	if(leg1.halfIsReached()){
		//updateBone(bone2, DeltaTime, FColor::Green);
	}

	

	//arms new, must be added
	FVector armOff = offset + FVector(0, 0, 100); // up offset for arms
	//arm1.build(GetWorld(), armOff, FColor::Purple, DeltaTime * 2);

	
	//neu braucht testing!
	MMatrix orientationLocation = ownLocation * ownOrientation; //lesen rückwärts:
	//rotation um eigene achse IM URSPRUNG
	//dann translation, aber rückwärts schreiben
	// R_T = T * R; //genau rückwärts rechnen
	MMatrix armMat = orientationLocation;
	FVector verticalOffset(0, 0, 100);
	armMat += verticalOffset;

	//orientation rotation matrix dann reingeben als first node / limb 
	arm1.build(GetWorld(), armMat, FColor::Purple, DeltaTime * 2);
	//leg2.build(GetWorld(), orientationLocation, FColor::Black, DeltaTime * 2); //new matrix offset
	//leg2.tickLegMotion(GetWorld(), DeltaTime, offset, FColor::Black); //VECTOR OFFSET
	leg2.tickLegMotion(GetWorld(), DeltaTime, orientationLocation, FColor::Black); //MATRIX OFFSET, TESTING NEEDED

	//fuss geht nicht?
	leg1.build(GetWorld(), orientationLocation, FColor::Red, DeltaTime * 2); //new matrix offset


	//debug draw of targets
	FVector t1 = targetA;
	FVector t2 = targetB;
	t1 += armOff;
	t2 += armOff;
	//DebugHelper::showLineBetween(GetWorld(), t1, t2, FColor::Green); //draw line of arm movement
	

	//arm pos follow testing (works as expected, muss aber refactured werden)
	debugDynamicArmTick(DeltaTime);

}



/// @brief updates a given bone (leg for walking)
/// @param bone 
/// @param deltaTime 
/// @param color 
void AIkActor::updateBone(BoneIk &bone, float deltaTime, FColor color){
	//FVector offset(1000, -1000, 200);
	//bone.tickLegMotion(GetWorld(), deltaTime, offset, color); //deprecated
}



void AIkActor::updatePositionBasedOnMovedDistance(BoneIk &trackedBone){
	//ACHTUNG: TODO: hier muss x und y gemessen werden damit
	//sich das bein / der körper auch in die richtige richtung bewegt!
	
	FVector pos = trackedBone.movedLastTick();
	float xMoved = pos.X; // say this is forward for now
	float yMoved = pos.Y;
	if (xMoved > 0)
	{
		//if x < 0 means the leg moved the body forward
		offset.X += xMoved;
		ownLocation.setTranslation(offset);
	}
	/*
	if(yMoved > 0){
		offset.Y += yMoved;
		ownLocation.setTranslation(offset);
	}*/

	
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
// seems to work as expected for arm.

void AIkActor::debugDynamicArmTick(float DeltaTime){

	//arm gets build in tick.

	float slow = 0.5f; //scales speed down?
	timeCopy += DeltaTime * direction * slow;
	
	//clamped von 0 to 1 und dreht die bewegungs richtung um wenn kante erreicht
	if(timeCopy < 0){
		direction *= -1; //wtf is direction
		timeCopy = 0;
	}
	if(timeCopy > 1){
		direction *= -1;
		timeCopy = 1;
	}


	FVector directionVecAll = (targetB - targetA); //AB = B - A // immer ganze grösse?
	FVector posNew = targetA + directionVecAll * timeCopy;
	FVector weight(1, 1, -1);
	arm1.rotateEndToTarget(posNew, weight);


}