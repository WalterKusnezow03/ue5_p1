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


/// @brief tries to find the internal skelleton of any actor, returns mesh component by pointer
/// if found, nullptr if not!
USkeletalMeshComponent* AskelletonController::findSkeleton(AActor *actorIn){
	if(actorIn == nullptr){
		return nullptr;
	}

	// Check if the actor has a skeletal mesh component
    USkeletalMeshComponent* SkeletalMeshComp = actorIn->FindComponentByClass<USkeletalMeshComponent>();
    if(SkeletalMeshComp != nullptr){
		//skelletonComponentPointer = SkeletalMeshComp;
		//DebugHelper::showScreenMessage("skelleton found A");
		return SkeletalMeshComp;
	}

	//is not called????
	TArray<UChildActorComponent *> childActors;
	actorIn->GetComponents<UChildActorComponent>(childActors);
	for (int i = 0; i < childActors.Num(); i++){
		UChildActorComponent *current = childActors[i];
		if(current != nullptr){
			AActor *actor = current->GetChildActor(); //retrieve child actor to acces its childs..
			if (actor != nullptr){
				
				SkeletalMeshComp = actor->FindComponentByClass<USkeletalMeshComponent>();
				if(SkeletalMeshComp != nullptr){
					//skelletonComponentPointer = SkeletalMeshComp;
					//DebugHelper::showScreenMessage("skelleton found B");

					return SkeletalMeshComp;
				}
			}
		}
	}

	return nullptr;
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
	if(skelletonComponentPointer == nullptr){
		skelletonComponentPointer = findSkeleton(this);
	}
	
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
	leftHandTargetMesh.reset();
	rightHandTargetMesh.reset();
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

/// @brief will attach left arm to an actor, THE POINTER WILL BE SAVED, DO NOT DESTROY
/// @param other
/// @param otherSocket
void AskelletonController::attachLeftArmTo(AActor *other, FName otherSocket){
	
	if(other != nullptr){
		//leftAttached = other;
		leftHandTargetMesh.reset();
		leftHandTargetMesh.init(other, otherSocket);
	}
}



void AskelletonController::attachRightArmTo(AActor *other, FName otherSocket){
	if(other != nullptr){
		rightHandTargetMesh.reset();
		rightHandTargetMesh.init(other, otherSocket);
	}
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


		if(leftHandTargetMesh.isInited() && rightHandTargetMesh.isInited()){


			FVector leftHandTarget = leftHandTargetMesh.socketLocation();
			FVector rightHandTarget = rightHandTargetMesh.socketLocation();

			//MUST BE TRANSFORMED TO LOCAL SPACE OF SKELLETON
			leftHandTarget = skelletonComponentPointer->GetComponentTransform()
														.InverseTransformPosition(leftHandTarget);

			rightHandTarget = skelletonComponentPointer->GetComponentTransform()
														.InverseTransformPosition(rightHandTarget);


			animInstance->SetLeftHandEffectorLocation(leftHandTarget); // tick position for anim instance
			animInstance->SetRightHandEffectorLocation(rightHandTarget);

		}
	}

	
}





/**
 * 
 * 
 * -- INNER CLASS SECTION --
 * 
 */
//constructor
AskelletonController::TargetMesh::TargetMesh(){

}
//destructor
AskelletonController::TargetMesh::~TargetMesh(){
}

void AskelletonController::TargetMesh::init(AActor *actor, FName socketNameIn){
	actorPointer = actor;
	targetSocket = socketNameIn;
	skeletonPointer = AskelletonController::findSkeleton(actor);
}

void AskelletonController::TargetMesh::reset(){
	actorPointer = nullptr;
	skeletonPointer = nullptr;
}

FVector AskelletonController::TargetMesh::socketLocation(){
	FVector output;
	if(actorPointer != nullptr && skeletonPointer != nullptr){
		
		FVector a = skeletonPointer->GetSocketLocation(targetSocket);
		//DrawDebugSphere(actorPointer->GetWorld(), a, 5.0f, 12, FColor::Red, false, 1.0f); //draws correctly
		return a;

	}
	return output;
}

/// @brief returns if the actor and skelleton are not null and the target can be accessed
/// @return is inited true false
bool AskelletonController::TargetMesh::isInited(){
	return actorPointer != nullptr && skeletonPointer != nullptr;
}