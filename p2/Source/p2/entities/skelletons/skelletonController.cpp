// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/util/AActorUtil.h"
#include "skelletonAnimInstance.h"
#include "p2/gamestart/assetManager.h"
#include "skelletonController.h"

// Sets default values
AskelletonController::AskelletonController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;// DISBALE TiCK FUNCTION TO FIX NOT HIDING ACTOR ISSUES
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
	updateIkPositions();
}


/// @brief returns the skelleton type
/// @return skelleton type (for example human)
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
		//DebugHelper::showScreenMessage("skelleton found A");
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
					//DebugHelper::showScreenMessage("skelleton found B");

					return;
				}
			}
		}
	}



}


FVector AskelletonController::boneWorldLocation(FName boneName){

	 // Check if the skeleton component is valid
    if (skelletonComponentPointer != nullptr) {
        // Get the bone index
        int32 BoneIndex = skelletonComponentPointer->GetBoneIndex(boneName);
        
        // Check if the bone index is valid
        if (BoneIndex != INDEX_NONE) {
            // Get the bone location in world space
            return skelletonComponentPointer->GetBoneLocation(boneName, EBoneSpaces::WorldSpace);
        }
    }
	return FVector(0, 0, 0);
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
	setupAnimInstance();

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
	
	//clean up
	leftAttached = nullptr;
}



/// @brief sets visible and collsion based on boolean
/// @param enable enable visibility collsion / or not
void AskelletonController::enableActiveStatus(bool enable){

	
	SetActorHiddenInGame(!enable); // propagates to children ? (should, just as weapon)
	//SetActorEnableCollision(enable);

	//Disable ticking fixes issue where actor is not set invisible
	PrimaryActorTick.bCanEverTick = enable; // Disable ticking
    SetActorTickEnabled(enable); // Stop ticking

}

/**
 * 
 * --- Testing needed from here: ---
 * 
 */


void AskelletonController::attachToBreastRight(AActor *other){
	if(other != nullptr){
		attachToSocket(other, breastRightSocket);
	}
}


/// @brief will attach other object to the right arm
/// @param other object to attach
void AskelletonController::attachToLeftArm(AActor *other){
	if(other != nullptr){
		attachToSocket(other, leftHandSocket);
	}
}


/// @brief will attach other object to the left arm
/// @param other object to attach
void AskelletonController::attachToRightArm(AActor *other){
	if(other != nullptr){
		attachToSocket(other, rightHandSocket); //is named like that!
	}
}

/// @brief will attach left arm to an actor, ThE POINTER WILL BE SAVED, DO NOT DESTROY
/// WILL BE CLEANED ON die(); !!
/// @param other
/// @param otherSocket
void AskelletonController::attachLeftArmTo(AActor *other, FName otherSocket){
	
	if(other != nullptr){
		leftAttached = other;
	}
}



void AskelletonController::attachRightArmTo(AActor *other, FName otherSocket){

}


/// @brief will completly detach other from everything (more universal of a function)
/// @param other other to detach
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

			//DebugHelper::showScreenMessage("attached to socket");
		}
	}
}

















/**
 * 
 * SETUP ANIM INSTANCE (if not set yet)
 * The Blue print Anim instance must be loaded from storage and an uclass received
 * make sure the blueprint has also set the correct parent class
 */
void AskelletonController::setupAnimInstance(){
	if(animInstance == nullptr && skelletonComponentPointer != nullptr){

		//TODO: LOAD ANIM INSTANCE FROM STORAGE!
		if(assetManager *a = assetManager::instance()){
			UClass *animInstanceBp = a->findUAnimInstanceBp(getType()); //of own type obviously
			
			//actually setting the class for this skelleton
			if(animInstanceBp != nullptr){
				// Set the animation instance class to the loaded blueprint class
				skelletonComponentPointer->SetAnimInstanceClass(animInstanceBp);

				// Get the newly created anim instance
				UskelletonAnimInstance* instance = Cast<UskelletonAnimInstance>(skelletonComponentPointer->GetAnimInstance());
				if (instance != nullptr) {
					animInstance = instance;
				}
			}
		}

		
	}
}



/// @brief tick the anim instance positions for the left arm for example
void AskelletonController::updateIkPositions(){
	if(animInstance != nullptr && skelletonComponentPointer != nullptr){


		if(leftAttached != nullptr){
			FVector leftHandWorld = boneWorldLocation("palm_01_L");
			FVector leftEllbowWorld = boneWorldLocation("hand_L");

			FVector weaponWorld = leftAttached->GetActorLocation(); //for now
			//will be in local space, which is wanted
			FVector leftHandTarget = weaponWorld - leftHandWorld; // AB = B - A;
			FVector leftEllTarget = weaponWorld - leftEllbowWorld;

			
			animInstance->SetLeftHandEffectorLocation(leftHandTarget);
			animInstance->SetLeftElbowJointTargetLocation(leftEllTarget);

			//DebugHelper::showScreenMessage(leftHandTarget);
		}
		
	}

	
}
