// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/BoneIk.h"
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
	arm1.roll(90); //gegen uhrzeiger sinn pos zahl
	arm1.pitch(-90); //im uhrzeiger sinn

	arm1.setEtha(0.7f);
}

// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateBone(leg1, DeltaTime, FColor::Red);
	updatePositionBasedOnMovedDistance(leg1);

	if(leg1.halfIsReached()){
		//updateBone(bone2, DeltaTime, FColor::Green);
	}

	

	//arms new, must be added
	FVector armOff = offset + FVector(0, 0, 100); // up offset for arms
	arm1.build(GetWorld(), armOff, FColor::Purple, DeltaTime * 2);
	//DebugHelper::showLineBetween(GetWorld(), FVector(0, 0, 0), armOff, FColor::Yellow);
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