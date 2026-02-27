// Fill out your copyright notice in the Description page of Project Settings.


#include "carriedItem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "p2/entities/EntityScript.h"
#include <list>

// Sets default values
AcarriedItem::AcarriedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	cameraPointer = nullptr;
	botPointer = nullptr;

	offset = FVector(-100, 100.0f, 0);

}

// Called when the game starts or when spawned
void AcarriedItem::BeginPlay()
{
	Super::BeginPlay();
	enableCollider(true);
	isVisible = true; //inital setting of visibilty, do not remove!
	SetupCarriedItemAxisContraints();
	FindFingerComponentsOnBeginPlay();
}

// Called every frame
void AcarriedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AcarriedItem::SetupCarriedItemAxisContraints(){
	carryInterfaceAxisConstraint.SetupNone();
}


void AcarriedItem::showScreenMessage(FString s){
	if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, s);
    }
}



/// @brief Only for player:
/// returns the offset vector of the sight and hipfire by value
/// @return offset vector
FVector AcarriedItem::getOffsetVector(){
	FVector pos = FVector(0, 0, 0);
	if(cameraPointer != nullptr){
		pos += cameraPointer->GetRightVector().GetSafeNormal() * 25;
	}
	return pos;
}



/// @brief will enable and disable the collider for the actor
/// @param enable 
void AcarriedItem::enableCollider(bool enable){
	SetActorEnableCollision(enable);
}



/// @brief allows the player to pickup the weapon
/// @param cameraRefIn 
void AcarriedItem::pickup(UCameraComponent *cameraIn){
	if(!isPickedupByPlayer() && cameraIn != nullptr && botPointer == nullptr){
		cameraPointer = cameraIn; // Assign the address of cameraRefIn to cameraRef
		botPointer = nullptr;
		enableCollider(false);

		showItem(true);
		
	
		//DEBUG HIDDEN FOR BONE CONTROLLER -- BONE CONTROLLER MANAGES ALL WEAPON POSITIONS IN WORLD
		/**
		 * this->AttachToComponent(cameraIn, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		 * */
		//DEBUG HIDDEN FOR BONE CONTROLLER

		OnPickup();
	}
}

/// @brief pickup method for bot, pointer will be saved!
/// @param actorIn actor bot  
void AcarriedItem::pickupBot(AActor *actorIn){
	if(botPointer == nullptr && actorIn != nullptr && cameraPointer == nullptr){
		botPointer = actorIn;
		cameraPointer = nullptr;
		enableCollider(false);

		showItem(true);

		OnPickup();
		// this->AttachToActor(botPointer, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	}
}

void AcarriedItem::OnPickup(){
	UnRegisterFromMiniMap();
	UpdatePreRequisiteTickOnPickup();
}

bool AcarriedItem::isPickedupByPlayer(){
	return cameraPointer != nullptr; //one must be set
	// return cameraPointer != nullptr;
}

bool AcarriedItem::isPickedUp(){
	return isPickedupByPlayer() || botPointer != nullptr;
}

/// @brief shows or hides the item
/// @param show 
void AcarriedItem::showItem(bool show){
	isVisible = show;

	SetActorHiddenInGame(!show);  // Hides the actor if 'show' is false
}


/// @brief shows or hides a child actor if not nullptr
/// @param component child actor to hide
/// @param show show true false
void AcarriedItem::showChildActor(UChildActorComponent* component, bool show){
	if(component != nullptr){
		component->SetHiddenInGame(!show);  // Hides the actor if 'show' is false
	}
}


/// @brief returns is active status for player (hidden or not)
/// @return is active (visible status)
bool AcarriedItem::isActive(){
	return isVisible;
}

/// @brief drops the item and enables the collider, removes all owners
void AcarriedItem::drop(){
	//deprecated, position managed by bone controller
	/*
	if(cameraPointer != nullptr){
		//detach from camera
		//this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false)); 
		//false for detaching complete physcics? / designed to uncouple everything
	}
	if(botPointer != nullptr){
		//detach from bot
		//ChildActor->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		//this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	}
	*/

	UpdatePreRequisiteTickOnDrop();
	
	cameraPointer = nullptr;
	botPointer = nullptr; //reset bot too, for both actors designed
	enableCollider(true);
	showItem(true);

	
}

void AcarriedItem::UpdatePreRequisiteTickOnPickup(){
	if(botPointer){
		AddTickPrerequisiteActor(botPointer);
	}
	if(cameraPointer){
		if(AActor *owner = cameraPointer->GetOwner()){
			AddTickPrerequisiteActor(owner);
		}
	}
}

void AcarriedItem::UpdatePreRequisiteTickOnDrop(){
	if(botPointer){
		RemoveTickPrerequisiteActor(botPointer);
	}
	if(cameraPointer){
		if(AActor *owner = cameraPointer->GetOwner()){
			RemoveTickPrerequisiteActor(owner);
		}
	}
}







void AcarriedItem::leftMouseDown(){

}

void AcarriedItem::leftMouseUp(){

}



// ---- IK INTERFACE API ----
// NEW IK HUMANOID ITEM POSITION DATA, data held by carried item
CarriedItemPositionData &AcarriedItem::getItemPositionDataRef(){
	//update containe, all scenes (hand and fingers are tracked inside!)
	handAndFingerPositionManager.UpdateContainer(internalCarriedItemPositionContainer);
	return internalCarriedItemPositionContainer;
}

FIKCarryInterfaceAxisConstraint &AcarriedItem::getAxisConstraint(){
	return carryInterfaceAxisConstraint;
}

void AcarriedItem::FindFingerComponentsOnBeginPlay(){
	handAndFingerPositionManager.UpdateFrom(this);
}




void AcarriedItem::UpdateActorTransform(
	FVector &location, 
	FRotator &rotation
){
	SetActorLocation(location);
	SetActorRotation(rotation);

	//DebugHelper::showScreenMessage("IIKCarried Item Update Transform", FColor::Cyan);
}

FVector AcarriedItem::LocalAnimationOffset(){
	return actorAnimationOffsetLocal();
}

// ---- IK INTERFACE API END ----





/// @brief returns the actor rotation as MMatrix to aligns the hands with
/// makes Roll 0.0f!
/// @return 
MMatrix AcarriedItem::handAlignForwardRotationMatrix(){
	FRotator rotator = GetActorRotation();
	rotator.Pitch *= -1; //?????? like this?
	rotator.Roll = 0.0f;
	MMatrix rotMat(rotator);
	return rotMat;
}






//new actor animation internal

///@brief will return the actor internal offset (NO rotation) to apply
/// from the bonecontroller motion queue (to combine)
FVector AcarriedItem::actorAnimationOffsetLocal(){
	return actorAnimationOffsetTicked;
}

/// @brief used by weapon to have kickback!
/// @param other 
void AcarriedItem::updateAnimationOffset(FVector &other){
	actorAnimationOffsetTicked = other;
}

void AcarriedItem::addToAnimationOffset(FVector &other){
	actorAnimationOffsetTicked += other;
}





//api bone controller sight offset
FVector AcarriedItem::sightOffsetNoRotation(){
	return verticalSightOffset;
}