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

int Aroom::getXScale(){
	return boxXScale;
}
int Aroom::getYScale(){
	return boxYScale;
}
int Aroom::getZScale(){
	return boxZScale;
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
	int zScale = (int)(Extent.Z * 2); //doesnt work for some reason


	//set iv for this room
	boxExtent = Extent;
	boxOrigin = Origin;
	boxXScale = xScale;
	boxYScale = yScale;
	boxZScale = zScale;

	DebugHelper::showScreenMessage("DEBUG ROOM Z SCALE ", boxZScale, FColor::Yellow);

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
	pos.Z -= boxZScale / 2;
	return pos;
}




void Aroom::disableWall(FVector &location){
	disableWall(location, nullptr);
}

/// @brief sort into the vector, find correct spot if based on int distance
/// vector wilöl be sorted with smallest float at the end
/// @param vec vector to check
/// @param actor new Actor to push
/// @param distance distance measured
void Aroom::sortIn(
	std::vector<TTouple<float, AActor*>> &vec, // by reference, nicht vergessen!
	AActor* actor,
	float distance,
	int sizetargeted
){
	if(actor != nullptr){
		if(vec.size() == 0){
			vec.push_back(TTouple<float, AActor*>(distance, actor));
		}else{

			//normal sortiert einfügen
			for (int i = vec.size() - 1; i >= 0; i--){

				float compare = vec.at(i).first();
				//smaller than prev
				if (distance <= compare) {
                    vec.insert(vec.begin() + i + 1, TTouple<float, AActor*>(distance, actor));
                    return;
                }else{
					//limit reached, bad pos
					if(i == vec.size() - sizetargeted - 1){
						return;
					}
				}
			}
			//very front reached
			vec.insert(vec.begin() + 0, TTouple<float, AActor*>(distance, actor));
			
		}
	}
}

/// @brief disables the closest wall to a point
/// @param location lcoation to replace a wall
/// @param bp to spawn (door or window) 
void Aroom::disableWall(FVector &location, UClass *bp){

	//refined version will:
	//check the x width of the blueprint
	//n = devide by 100 for meters
	//create a vector of count n
	//found n most closest elements
	//disable them all
	//spawn the bp at the center of those n elements
	if(wallActors.Num() > 0){
		int xcopy = 0; //real scale
		int ycopy = 0;
		int zcopy = 0;
		AActorUtil::calculateActorBounds(GetWorld(), bp, xcopy, ycopy, zcopy);

		//scale to index
		int limit = xcopy / 100; //x is the width in the blueprints
		if(true){
			DebugHelper::showScreenMessage("ROOM LIMIT ", xcopy, FColor::Cyan);
		}
		

		std::vector<TTouple<float, AActor*>> vec;

		for (int i = 0; i < wallActors.Num(); i++)
		{
			AActor *a = wallActors[i];
			if (a != nullptr)
			{
				FVector current = a->GetActorLocation();
				float distTmp = FVector::Dist(location, current);
				sortIn(vec, a, distTmp, limit);
			}
		}

		int startIndex = vec.size() - limit ; // -1 ?? no ; //-1 because 0 based //closest will be in closest spots
		FVector center(0, 0, 0);
		FRotator orient;
		if (startIndex >= 0)
		{
			orient = vec.at(startIndex).last()->GetActorRotation();

			
			for (int i = startIndex; i < vec.size(); i++){
				AActor *pointer = vec.at(i).last();
				center += pointer->GetActorLocation();
				
				//disable the wall
				pointer->SetActorHiddenInGame(true);
				pointer->SetActorEnableCollision(false);

				//find an remove from walls
				for (int j = 0; j < wallActors.Num(); j++){
					if(wallActors[j] == pointer){
						wallActors.RemoveAt(j);
						j = wallActors.Num();
					}
				}
			}

		}
		center /= limit;

		if(EntityManager *e = EntityManager::instance()){
			AActor *spawned = e->spawnAactor(GetWorld(), bp, center);
			if(spawned != nullptr){
				spawned->SetActorLocation(center);
				spawned->SetActorRotation(orient);
			}
		}
	}

}




/// @brief will process all door positions of a room
/// @param positions to check and all walls will be replaced with a uclass which is spawned
/// thats why the uclass should be a door or a window, etc.
/// all positions are Z irrelevant, the room will take care of applying world space to the relative coordinates
void Aroom::processPositionVectorsAndReplaceWall(std::vector<FVector> &toPositionVector, UClass *bp){

	//findWalls();

	//enable disable doors based on bottom left + offset
	FVector bottomLeft = bottomLeftCorner();

	for (int i = 0; i < toPositionVector.size(); i++){
		FVector relativeDoorPos = bottomLeft + toPositionVector.at(i); //A + (B - A) //positions
		relativeDoorPos.Z = bottomLeft.Z;

		FVector debugUp = relativeDoorPos + FVector(0, 0, 100);
		FVector debugDown = relativeDoorPos;
		//+FVector(0, 0, -100);
		if(Aroom::DEBUG_DRAW){
			DebugHelper::showLineBetween(GetWorld(), debugDown, debugUp, FColor::Red);
		}


		disableWall(relativeDoorPos, bp);
	}

	debugShowOutline();
}

/// @brief debug method to show rooms ouline
void Aroom::debugShowOutline(){
	if(!Aroom::DEBUG_DRAW){
		return;
	}
	std::vector<FVector> corners = debugAllCorners();
	for (int i = 0; i < corners.size(); i++){
		int prevIndex = i - 1;
		if(prevIndex < 0){
			prevIndex = corners.size() - 1;
		}
		FVector pos = corners.at(i); // A + (B - A) //positions
		FVector prev = corners.at(prevIndex);
		//DebugHelper::showLineBetween(GetWorld(), pos, prev, FColor::Green);
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




std::vector<FVector> Aroom::allCorners(){

	std::vector<FVector> returned;
	FVector bl = bottomLeftCorner() + FVector(0,0,0);
	FVector tl = bl + FVector(0, boxYScale, 0);
	FVector br = bl + FVector(boxXScale, 0, 0);
	FVector tr = bl + FVector(boxXScale, boxYScale, 0);

	returned.push_back(bl);
	returned.push_back(tl);
	returned.push_back(tr);
	returned.push_back(br);


	return returned;
}


void Aroom::spawnWalls(UClass *bp){

	//skip staircase for debugging
	if(type == roomtypeEnum::staircase){
		return;
	}

	int OFFSET_FIX = 50; //50cm offset fix because the center of the bp is the center and not start
	//(while the bp being still 100cm wide)

	EntityManager *e = EntityManager::instance();
	if(e != nullptr && bp != nullptr){
		std::vector<FVector> corners = allCorners();
		corners.push_back(corners.front()); //create circle for simplicity

		//go along corners, rotate 90 degree each flip
		//spawn walls
		//always +100 cm
		for (int i = 1; i < corners.size(); i++){
			FVector from = corners.at(i - 1);
			FVector to = corners.at(i);

			FVector connectNormalized = (to - from).GetSafeNormal();
			connectNormalized.Z = 0; //prevent high issues just to make sure

			FVector lookDir = connectNormalized; //keep like this, is rotated correctly in blue print!, look in moving dir
			//to be rotated correctly.

			int distance = FVector::Dist(from, to);
			int one_meter = 100;

			for (int cm = 0; cm < distance; cm += one_meter){
				//scale along the normal in 100cm steps and initial offset fix of half width (50cm)
				FVector spos = from + connectNormalized * (cm + OFFSET_FIX);

				//CAUTION! IS EXACTLY TOO MUCH, BUT WHY! NO ROTATION CORRECT, REASON: UNKNOWN!
				FRotator lookRotation = FVectorUtil::lookAt(spos, spos + lookDir); 

				//spawn actor, set location and rotation
				AActor *actor = e->spawnAactor(GetWorld(), bp, spos);
				if(actor != nullptr){
					actor->SetActorRotation(lookRotation);
					actor->SetActorLocation(spos);

					wallActors.Add(actor);
				}
			}
		}
	}

	
}
