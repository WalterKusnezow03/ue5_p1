// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/util/AActorUtil.h"
#include "p2/rooms/roomParts/jumpableActor.h"

// Sets default values
AjumpableActor::AjumpableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	playerPointer = nullptr;
}

// Called when the game starts or when spawned
void AjumpableActor::BeginPlay()
{
	Super::BeginPlay();
	init();
}

void AjumpableActor::init(){
	findTargetLocations();
	if(referenceManager *r = referenceManager::instance()){
		playerPointer = r->getPlayerPointer();
	}
}

// Called every frame
void AjumpableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(playerPointer == nullptr){
		if(referenceManager *r = referenceManager::instance()){
			playerPointer = r->getPlayerPointer();
		}
	}

	bool playerIsInMovingQeue = false;

	//move all actors
	int i = 0;
	int size = moving.size();
	while(size > 0 && i < size){
		TTouple<AActor *, FVector> &touple = moving.at(i);
		AActor *a = touple.first();
		if(a == nullptr){
			moving.erase(moving.begin() + i);
		}else{
			//check for player to display or not display ui
			if(a == playerPointer){
				playerIsInMovingQeue = true;
			}

			//move
			FVector aPos = a->GetActorLocation();
			FVector &target = touple.last();
			if (!hasReached(aPos, target))
			{
				move(DeltaTime, a, target);
				i++; //only increment here
			}else{
				moving.erase(moving.begin() + i);
			}
		}

		size = moving.size();
	}

	if(!playerIsInMovingQeue && playerPointer != nullptr){
		//display ui to player
		FVector p = playerPointer->GetActorLocation();
		if(inRange(p)){
			//display ui to player
		}
	}
}

void AjumpableActor::move(float DeltaTime, AActor *a, FVector &target){

	//x(t) = x0 + v0 t + 1/2 at^2
	FVector x0 = a->GetActorLocation();
	int v0scalar = metersPerSecond; // lets say thats constant at any time
	FVector v0dir = (target - x0).GetSafeNormal(); // ab = b - a
	//lets say not gravity

	FVector xt = x0 + v0scalar * DeltaTime * v0dir;
	a->SetActorLocation(xt);
}

bool AjumpableActor::inRange(FVector &pos){
	if(FVector::Dist(pos, GetActorLocation()) <= MAX_DISTANCE){
		return true;
	}
	return false;
}

bool AjumpableActor::hasReached(FVector &a, FVector &b){
	int epsilon = 50;
	return (FVector::Dist(a, b) <= epsilon);
}

void AjumpableActor::jumpOver(AActor *actor){
	if(actor != nullptr){

		//dont allowed to be already in list
		for (int i = 0; i < moving.size(); i++){
			if(moving.at(i).first() == actor){ //not allowed to be in the process lsit already
				return;
			}
		}

		//start the sequence, maybe create a map to update the anchors position
		FVector pos = actor->GetActorLocation();
		FVector &target = findFurthestTarget(pos); //natürlcih eine referenz, referenzen sind unser freund.
		moving.push_back(TTouple<AActor *, FVector>(actor, target));
	}
}

/// @brief finds all target locations inside the actor blueprint
void AjumpableActor::findTargetLocations(){

	FString namepart = TEXT("target");
	TArray<UChildActorComponent*> out;
	AActorUtil::findAllChildsByName(*this, namepart, out);
	if(out.Num() > 0){
		for (int i = 0; i < out.Num(); i++){
			UChildActorComponent *p = out[i];
			if (p != nullptr)
			{
				AActor *a = p->GetChildActor();
				if(a != nullptr){
					FVector location = a->GetActorLocation();
					targets.push_back(location);
				}
			}
		}
	}
}

/// @brief 
/// @param asked 
/// @return 
FVector& AjumpableActor::findFurthestTarget(FVector &asked){
	FVector &close = asked;
	float distance = 0; // FVectorUtil::infinity();
	for (int i = 0; i < targets.size(); i++)
	{
		FVector &compare = targets.at(i); // int &b = a; remember. 
		float newDist = FVector::Dist(asked, compare);
		if (newDist > distance)
		{
			distance = newDist;
			close = compare;
		}
	}
	return close;
}