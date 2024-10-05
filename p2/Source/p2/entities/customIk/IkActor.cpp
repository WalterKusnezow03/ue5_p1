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

	leg1.setupBones(legScaleMeters);
	leg2.setupBones(legScaleMeters);

	float armScale = legScaleMeters;
	arm1.setupBones(armScale);
	
	
	
	//arm1.rotateFirstLimbDeg(-90, -90, 0);

	//rotate, then etha works good
	//arm1.rotateFirstLimbDeg(-90, -90, 0);
	//arm1.setEtha(0.5f); //0 being fully extended (?)


	
	//arm1.rotateFirstLimbDeg(-90, -90, 0);
	//arm1.rotateFirstLimbDeg(-20, -20, 0);

	//arm1.setEtha(0.5f); // 0 being fully extended (?)

	//arm1.setEtha(0.5f); //0 being fully extended (?)
	
	arm1.rotateFirstLimbDeg(0, -110, 10);
	arm1.rotateFirstLimbDeg(0, 20, 10);
	//arm1.rotateFirstLimbDeg(0, 0, 0);



	//debug
	leg2.rotateFirstLimbDeg(0, -90, 10);
}

// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateBone(leg1, DeltaTime, FColor::Red);
	//updatePositionBasedOnMovedDistance(leg1);

	if(leg1.halfIsReached()){
		//updateBone(bone2, DeltaTime, FColor::Green);
	}

	

	//arms new, must be added
	FVector armOff = offset + FVector(0, 0, 100); // up offset for arms
	arm1.build(GetWorld(), armOff, FColor::Purple, DeltaTime * 2);

	leg2.build(GetWorld(), armOff, FColor::Black, DeltaTime * 2);
	
	//draw forward line to approve arm is correct
	DebugHelper::showLineBetween(GetWorld(), armOff, armOff + FVector(100,0,0), FColor::Green, DeltaTime * 2);
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
	}
	
}






/// @brief look at a location
/// @param TargetLocation target to look at
void AIkActor::LookAt(FVector TargetLocation) 
{
    // Calculate the rotation needed to look at the target location
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);

    // Optionally, you can set only the yaw to rotate around the Z-axis
    //LookAtRotation.Pitch = 0.0f;
    //LookAtRotation.Roll = 0.0f;

    // Apply the rotation to the actor
    //SetActorRotation(LookAtRotation);

	float zDegree = LookAtRotation.Yaw;

}