// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/util/AActorUtil.h"
#include "p2/entityManager/EntityManager.h"
#include "room.h"

// Sets default values
Aroom::Aroom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aroom::BeginPlay()
{
	Super::BeginPlay();
	findDoors();
	findWalls();
	calculateActorBounds();
}

// Called every frame
void Aroom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// @brief returns the room type for this room
/// @return room type from enum
roomtypeEnum Aroom::readType(){
	return type;
}

/// @brief will calculate and set room bounds, is ideally called on begin play!
void Aroom::calculateActorBounds(){
	/**
    * from doc:
    *  AActor::GetActorBounds(...){} 
    	
		virtual void GetActorBounds  
        &40;  
            bool bOnlyCollidingComponents,  
            FVector & Origin,  
            FVector & BoxExtent,  
            bool bIncludeFromChildActors  
        &41; const  

     */
	FVector Origin;
    FVector Extent;
    this->GetActorBounds(true, Origin, Extent); //leg mir das da rein prinzip


    int xScale = (int)(Extent.X * 2);
    int yScale = (int)(Extent.Y * 2);
	int zScale = (int)(Extent.Z * 2);


	//set iv for this room
	boxExtent = Extent;
	boxOrigin = Origin;
	boxXScale = xScale;
	boxYScale = yScale;
	boxZScale = zScale;

	//FVector newLocation = GetActorLocation() + (GetActorLocation() - bottomLeftCorner());
	//SetActorLocation(newLocation);
}

/// @brief calculates the bottom left corner based on box extent, calculateActorBounds() must be called any time before!
/// @return bottom left corner of this room
FVector Aroom::bottomLeftCorner(){
	FVector pos = boxOrigin;
	//FVector pos = GetActorLocation();
	pos.X -= boxXScale / 2;
	pos.Y -= boxYScale / 2;
	//pos.Z -= boxZScale / 2;
	return pos;
}





void Aroom::findDoors(){
	TArray<UChildActorComponent *> childs; //create a TArray of the targeted type
	GetComponents<UChildActorComponent>(childs); //collect all types with GetComponents<dt>(array) method
	if(childs.Num() > 0){
		for (int i = 0; i < childs.Num(); i++){
			if(childs[i] != nullptr){
				FString name = childs[i]->GetName();


				if(name.Contains("door")){
					DebugHelper::showScreenMessage("door FOUND", FColor::Red);

					//doorPositions.push_back(childs[i]->GetActorLocation());
				}
			}
		}
	}
}

void Aroom::findWalls(){
	TArray<UChildActorComponent *> container;
	FString name = FString::Printf(TEXT("wall"));
	AActorUtil::findDirectChildsByName(*this, name, container);
	
	//cast walls to aactor
	int added = 0;

	for (int i = 0; i < container.Num(); i++)
	{
		if(container[i] != nullptr){
			walls.Add(container[i]);
			/*
			AActor *a = Cast<AActor>(container[i]);
			if(a != nullptr){
				walls.Add(a);
				added++;
			}else{
				DebugHelper::showScreenMessage("cast failed", FColor::Red);
			}*/
		}
	}

	//get child actors
	for(UChildActorComponent *comp : container){ //UChildActor component is the Child actor visble in the actor tree
		if(comp != nullptr){
			AActor *actor = comp->GetChildActor(); //Acess it as Actor: call this method for the UChildActorComponent
			if(actor != nullptr){
				wallActors.Add(actor);
				added++;
			}
		}
		
	}

	if(added > 0){
		//DebugHelper::showScreenMessage("collected walls final ", added);
	}
}


void Aroom::disableWall(FVector &location){
	disableWall(location, nullptr);
}

/// @brief disables the closest wall to a point
/// @param location 
void Aroom::disableWall(FVector &location, UClass *doorBp){

	if(wallActors.Num() > 0){

		//find closest wall and disable it
		AActor *closest = wallActors[0];
		float closestDist = std::numeric_limits<float>::max();

		for(AActor *a : wallActors){
			if(a != nullptr){
				FVector current = a->GetActorLocation();
				float distTmp = FVector::Dist(location, current);
				if(distTmp < closestDist){
					closestDist = distTmp;
					closest = a;
				}
			}
		}


		if(closestDist <= 100 && closest != nullptr){ //100cm als thresehold
			closest->SetActorHiddenInGame(true);
			closest->SetActorEnableCollision(false);
			//DebugHelper::showScreenMessage("disabled a wall");

			//spawn a door
			if(EntityManager *e = EntityManager::instance()){
				if(doorBp != nullptr){
					FVector locationCopy = closest->GetActorLocation();
					FRotator rotationCopy = closest->GetActorRotation();
					AActor *spawnedDoor = e->spawnAactor(GetWorld(), doorBp, locationCopy);
					if(spawnedDoor != nullptr){
						spawnedDoor->SetActorLocation(locationCopy);
						spawnedDoor->SetActorRotation(rotationCopy);
					}
				}
			}
			
		}
	}
	


}




/// @brief will process all door positions of a room
/// @param positions 
void Aroom::processDoorPositionVectors(std::vector<FVector> &toPositionVector, UClass *doorBp){

	//enable disable doors based on bottom left + offset
	FVector bottomLeft = bottomLeftCorner();

	for (int i = 0; i < toPositionVector.size(); i++){
		FVector relativeDoorPos = bottomLeft + toPositionVector.at(i); //A + (B - A) //positions

		FVector debugUp = relativeDoorPos + FVector(0, 0, 100);
		FVector debugDown = relativeDoorPos + FVector(0, 0, -100);
		DebugHelper::showLineBetween(GetWorld(), debugDown, debugUp, FColor::Red);
		//DebugHelper::showLineBetween(GetWorld(), bottomLeft, debugUp, FColor::Green);

		//find walls to disable
		//50cm * 2 is the default width
		disableWall(relativeDoorPos, doorBp);
	}

	debugShowOutline();
}

/// @brief debug method to show rooms ouline
void Aroom::debugShowOutline(){
	std::vector<FVector> corners = debugAllCorners();
	for (int i = 0; i < corners.size(); i++){
		int prevIndex = i - 1;
		if(prevIndex < 0){
			prevIndex = corners.size() - 1;
		}
		FVector pos = corners.at(i); // A + (B - A) //positions
		FVector prev = corners.at(prevIndex);
		DebugHelper::showLineBetween(GetWorld(), pos, prev, FColor::Green);
	}	

}

std::vector<FVector> Aroom::debugAllCorners(){

	std::vector<FVector> returned;
	FVector bl = bottomLeftCorner() + FVector(0,0,10);
	FVector tl = bl + FVector(0, boxYScale, 0);
	FVector br = bl + FVector(boxXScale, 0, 0);
	FVector tr = bl + FVector(boxXScale, boxYScale, 0);

	returned.push_back(bl);
	returned.push_back(tl);
	returned.push_back(tr);
	returned.push_back(br);


	return returned;
}