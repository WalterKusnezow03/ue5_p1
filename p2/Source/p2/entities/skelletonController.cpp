// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/util/AActorUtil.h"
#include "skelletonController.h"

// Sets default values
AskelletonController::AskelletonController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;// DISBALE TiCK FUNCTION TO FIX NOT HIDING ACTOR ISSUES
	skelletonComponentPointer = nullptr;

}

// Called when the game starts or when spawned
void AskelletonController::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AskelletonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

skelletonControllerEnum AskelletonController::getType(){
	return type;
}


/// @brief tries to find the internal skelleton and saves it in the pointer
void AskelletonController::findSkeletonOnStart(){

	if(skelletonComponentPointer != nullptr){
		return;
	}

	// Check if the actor has a skeletal mesh component
    USkeletalMeshComponent* SkeletalMeshComp = FindComponentByClass<USkeletalMeshComponent>();
    if(SkeletalMeshComp != nullptr){
		skelletonComponentPointer = SkeletalMeshComp;
		DebugHelper::showScreenMessage("skelleton found A");
		return;
	}

	//is not called????
	TArray<UChildActorComponent *> childActors;
	GetComponents<UChildActorComponent>(childActors);
	for (int i = 0; i < childActors.Num(); i++){
		UChildActorComponent *current = childActors[i];
		if(current != nullptr){
			AActor *actor = current->GetChildActor(); //retrieve child actor to acces its childs..
			if (actor != nullptr)
			{
				
				SkeletalMeshComp = actor->FindComponentByClass<USkeletalMeshComponent>();
				if(SkeletalMeshComp != nullptr){
					skelletonComponentPointer = SkeletalMeshComp;
					DebugHelper::showScreenMessage("skelleton found B");

					return;
				}
			}
		}
	}
}







/**
 * Damage interface methods / life cycle for death
 */
teamEnum AskelletonController::getTeam(){
	if(owningEntity != nullptr){
		return owningEntity->getTeam();
	}
	return teamEnum::none; //none returned
}

void AskelletonController::setTeam(teamEnum teamIn){
	//dont do anything here
}

/// @brief sets the owner of the skelleton and will damage be dealt to
/// also ENABLES the skelleton collsion and visibilty
/// @param owner 
void AskelletonController::setOwner(IDamageinterface *owner){
	owningEntity = owner;

	//enableActiveStatus(true);
	findSkeletonOnStart();
	enableActiveStatus(true);
}


void AskelletonController::takedamage(int d){
	DebugHelper::showScreenMessage("skelleton damage");
	if (owningEntity != nullptr)
	{
		owningEntity->takedamage(d);
	}
}


void AskelletonController::takedamage(int d, FVector &from){
	takedamage(d);
}

/// @brief will detach it self from any actor, and release to the entity manager
void AskelletonController::die(){

	DebugHelper::showScreenMessage("skelleton die", FColor::Red);
	this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); // detach from everything
	owningEntity = nullptr;

	if(EntityManager *e = worldLevel::entityManager()){
		e->add(this);
	}

	enableActiveStatus(false); //does not have ffect, bool yes, real visibilty no, transform neither, unreal engine bug!
	
	
}



/// @brief sets visible and collsion based on boolean
/// @param enable enable visibility collsion / or not
void AskelletonController::enableActiveStatus(bool enable){

	
	SetActorHiddenInGame(!enable); // propagates to children ? (should, just as weapon)
	//SetActorEnableCollision(enable);

	return;
	/*
	if(skelletonComponentPointer != nullptr){
		DebugHelper::showScreenMessage("Setting visibility to: " + FString::FromInt(enable));
		skelletonComponentPointer->SetHiddenInGame(!enable, true);
		skelletonComponentPointer->MarkRenderStateDirty();
		
	}*/
}

/**
 * 
 * --- Testing needed from here: ---
 * 
 */


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





