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

	//init offset
	offset = FVector(1000, -1000, 200);

	bone1.setupBones(legScaleMeters);
	bone2.setupBones(legScaleMeters);
}

// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateBone(bone1, DeltaTime, FColor::Red);
	updatePositionBasedOnMovedDistance(bone1);

	if(bone1.halfIsReached()){
		//updateBone(bone2, DeltaTime, FColor::Green);
	}
	
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

	DebugHelper::showScreenMessage("moved ", (int)offset.X, FColor::Red);
}