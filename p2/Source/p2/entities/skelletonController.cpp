// Fill out your copyright notice in the Description page of Project Settings.


#include "skelletonController.h"

// Sets default values
AskelletonController::AskelletonController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = false; //disbaled, not needed, is managed by owning entity

	//might be set to false
	skelletonComponentPointer = nullptr;

	//SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    //RootComponent = SkeletalMeshComponent; // Set as root component
}

// Called when the game starts or when spawned
void AskelletonController::BeginPlay()
{
	Super::BeginPlay();
	findSkeletonOnStart();
}

// Called every frame
void AskelletonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/**
 * 
 * --- Testing needed from here: ---
 * 
 */

/// @brief tries to find the internal skelleton and saves it in the pointer
void AskelletonController::findSkeletonOnStart(){

	// Check if the actor has a skeletal mesh component
    USkeletalMeshComponent* SkeletalMeshComp = FindComponentByClass<USkeletalMeshComponent>();
    if(SkeletalMeshComp != nullptr){
		skelletonComponentPointer = SkeletalMeshComp;
	}
}



void AskelletonController::attachLeftArm(AActor *other){
	if(other != nullptr){
		attachToSocket(other, "leftarmsocket");
	}
}

void AskelletonController::attachRightArm(AActor *other){
	if(other != nullptr){
		attachToSocket(other, "rightarmsocket");
	}
}

void AskelletonController::detach(AActor *other){
	if(other != nullptr){
		other->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); //what is true for?
	}
}




/// @brief tries to attach a actor to a socket if the skelleton is not nullptr and 
/// the socket is found
/// @param actorToAttach actor to attach 
/// @param socketName socket (name) to attach to
void AskelletonController::attachToSocket(AActor *actorToAttach, FName socketName){
	if(skelletonComponentPointer != nullptr){
		if (skelletonComponentPointer->DoesSocketExist(socketName))
		{
			//attach
			actorToAttach->AttachToComponent(
				skelletonComponentPointer, 
				FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
				socketName
			);
		}
	}
}





