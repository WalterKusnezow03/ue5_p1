// Fill out your copyright notice in the Description page of Project Settings.


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
	calculateActorBounds();
}

// Called every frame
void Aroom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	
}

/// @brief calculates the bottom left corner based on box extent, calculateActorBounds() must be called any time before!
/// @return bottom left corner of this room
FVector Aroom::bottomLeftCorner(){
	FVector pos = boxOrigin;
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



/// @brief will process all door positions of a room
/// @param positions 
void Aroom::processDoorPositionVectors(std::vector<FVector> &toPositionVector){

	//enable disable doors based on bottom left + offset
	FVector bottomLeft = bottomLeftCorner();

	for (int i = 0; i < toPositionVector.size(); i++){
		FVector relativeDoorPos = bottomLeft + toPositionVector.at(i); //A + (B - A) //positions

		FVector debugUp = relativeDoorPos + FVector(0, 0, 200);
		//DebugHelper::showLineBetween(GetWorld(), relativeDoorPos, debugUp, FColor::Green);
	}

	debugShowOutline();
}

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
	FVector bl = bottomLeftCorner() + FVector(0,0,100);
	FVector tl = bl + FVector(0, boxYScale, 0);
	FVector br = bl + FVector(boxXScale, 0, 0);
	FVector tr = bl + FVector(boxXScale, boxYScale, 0);

	returned.push_back(bl);
	returned.push_back(tl);
	returned.push_back(tr);
	returned.push_back(br);


	return returned;
}