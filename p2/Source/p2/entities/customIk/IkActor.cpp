// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/BoneIk.h"
#include "Kismet/KismetMathLibrary.h"
#include <cmath>
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
	float legScaleCM = 200;


	//init offset for now
	FVector offset(1000, -1000, 200);
	ownLocation.setTranslation(offset);
	offset -= FVector(0, 0, legScaleCM);
	ownLocationFoot.setTranslation(offset);

	
	leg1.setupBones(legScaleCM);
	leg2.setupBones(legScaleCM);
	leg2.setDegreeInital(180); //testing for offset movement

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
	
	//testing values for arm movement
	targetA = FVector(1.0f, -0.5f, 2); //z 0  
	targetB = FVector(1.5f, 1.0f, -1);
	targetB = targetA - FVector(0, 0, 1); //testing

	targetA *= 100; //auf cm skalieren.
	targetB *= 100;

	//testing
	//ownOrientation.yawRadAdd(MMatrix::degToRadian(90));

	//testing look at, works
	//FVector target(0, 0, 0);
	//LookAt(target);

	//float initialDegree = 0; //45
	//ownOrientation.yawRadAdd(MMatrix::degToRadian(initialDegree));
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
	//updatePositionBasedOnMovedDistance(leg2); //movement for own location
	//updatePositionBasedOnMovedDistance(leg1);


	
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
	
	leg2.tickLegMotion(GetWorld(), DeltaTime, orientationLocation, FColor::Black); //MATRIX OFFSET
	updatePositionBasedOnMovedDistance(leg2);




	//debug draw of targets
	//arms new, must be added
	FVector armOff = orientationLocation.getTranslation() + FVector(0, 0, 100); // up offset for arms
	
	FVector t1 = targetA;
	FVector t2 = targetB;
	t1 += armOff;
	t2 += armOff;
	DebugHelper::showLineBetween(GetWorld(), t1, t2, FColor::Green, DeltaTime * 2); //draw line of arm movement
	
	

	//arm pos follow testing (works as expected, muss aber refactured werden)
	debugDynamicArmTick(DeltaTime);





	//NEW
	MMatrix legMat = orientationLocation;
	//verticalOffset = FVector(0, 0, -200);
	//legMat += verticalOffset;

	

	FVector targetHip = FVector(1.0f, 0.0f, 1.0f);
	targetHip = FVector(0, 0, 1.0f);
	targetHip *= 100;

	FVector weight(- 1, 0, 0);
	/*
	disbaled in favour of testing dynamic movement

	leg1.rotateStartToTargetAndBuild(
		GetWorld(),
		targetHip,
		weight,
		legMat,
		FColor::Emerald, //is disabled intenally for better debug understanding
		DeltaTime * 2.0f
	);*/
	

	//compare
	/*
	weight = FVector(1, 0, 0);
	FVector targetC = FVector(1.0f, 0.0f, -1.5f);
	targetC *= 100;
	leg2.rotateEndToTarget(targetC, weight);
	leg2.build(GetWorld(), orientationLocation, FColor::Black, DeltaTime * 2); //new matrix offset
	*/
}



/// @brief updates a given bone (leg for walking)
/// @param bone 
/// @param deltaTime 
/// @param color 
void AIkActor::updateBone(BoneIk &bone, float deltaTime, FColor color){
	//FVector offset(1000, -1000, 200);
	//bone.tickLegMotion(GetWorld(), deltaTime, offset, color); //deprecated
}


//CAUTION, will be deprecated (?)
void AIkActor::updatePositionBasedOnMovedDistance(BoneIk &trackedBone){
	//ACHTUNG: TODO: hier muss x und y gemessen werden damit
	//sich das bein / der körper auch in die richtige richtung bewegt!
	
	FVector pos = trackedBone.movedLastTick();
	pos.X = std::max(0.0, pos.X);
	pos.Y = std::max(0.0, pos.Y);

	pos.Z = 0; //block Z for now.
	ownLocation += pos;
	ownLocationFoot += pos;

	/*
	ownLocation.setTranslation(offset);
	


	FVector offset = ownLocation.getTranslation();
	FVector offsetB = ownLocationFoot.getTranslation();

	float xMoved = pos.X; // say this is forward for now
	float yMoved = pos.Y;
	if (xMoved > 0)
	{
		//if x < 0 means the leg moved the body forward
		offset.X += xMoved;
		ownLocation.setTranslation(offset);
	}
	
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
	MMatrix transform = currentTransform();
	FVector location = transform.getTranslation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(location, TargetLocation);

	//FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
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



	//debug offste
	//RT = T * R
	//MMatrix legMat = ownOrientation * ownLocation;
	MMatrix legMat = currentFootTransform(); //foot instead of hip. Is intiial offset!
	weight = FVector(1, 0, 0);
	FVector target = posNew;
	
	leg1.rotateStartToTargetAndBuild(
		GetWorld(),
		target,
		weight,
		legMat,
		ownLocation,
		FColor::Emerald, //is disabled intenally for better debug understanding
		DeltaTime * 2.0f
	);


}


/// @brief returns the current translation and rotation in world in correct order as expected
/// HIP PIVOT TRANSFORM ONLY
/// @return current Transform Matrix by value
MMatrix AIkActor::currentTransform(){
	// TR = R * T;
	// RT = T * R;
	//gewünscht ist jetzt erst an end punkt und dann um eigene
	//achse zu rotieren, also (geprüft!):
	MMatrix rotationTransform = ownLocation * ownOrientation;
	return rotationTransform;
}

/// @brief returns the current translation and rotation in world in correct order as expected
/// LOWER LEG PIVOT TRANSFORM ONLY
/// @return current Transform Matrix by value
MMatrix AIkActor::currentFootTransform(){
	//RT = T * R
	MMatrix rotationTransform = ownLocationFoot * ownOrientation;
	return rotationTransform;
}
