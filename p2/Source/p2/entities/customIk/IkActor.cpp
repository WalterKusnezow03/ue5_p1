// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/bonePackage/FabrikBone.h"
#include "Kismet/KismetMathLibrary.h"
#include <cmath>
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"
#include "p2/entities/customIk/animation/DoubleKeyFrameAnimation.h"
#include "p2/gameStart/assetManager.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entities/customIk/IkActor.h"
#include "p2/entities/customIk/bonePackage/TwoBone.h"

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


	//init offset for now
	FVector offset(1000, -1000, 200);
	ownLocation.setTranslation(offset); //hip location initial
	
	
	//debug testign controller, init offset
	hipController.SetLocation(offset);

	//testing inital rotation
	int DEBUGROATATE = 0;
	ownOrientation.yawRad(MMatrix::degToRadian(DEBUGROATATE));
	
	/*
	//debug matrix inverse bestimmen (klappt):
	MMatrix current = currentTransform();
	MMatrix invers = current.jordanInverse(); // current.adjunktInverse();
	MMatrix identitity = current * invers;

	FString message1 = FString::Printf(TEXT("debug matrix inverse matrix: "));
	message1 += invers.asString();
	DebugHelper::logMessage(message1);

	FString message = FString::Printf(TEXT("debug matrix Sollte einheits matrix ergeben: "));
	message += identitity.asString();
	DebugHelper::logMessage(message);
	*/

	hipController.SetControllerState(BoneControllerStates::locomotion);

	/*
	BoneController b(100.0f);
	shoulderController = b;
	shoulderController.SetControllerState(BoneControllerStates::none);
	*/


	//testing 
	FVector shoulderOffsetVec(0, 0, 100);
	hipToShoulderMatrix.setTranslation(shoulderOffsetVec);



	//testing fabrik
	/*
	float debugDisplayTime = 100.0f;
	FabrikBone debugFabrikBone;
	debugFabrikBone.moveEndToTarget(GetWorld(), FVector(200,-300,200), ownLocation, debugDisplayTime);
	*/
}

// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	hipController.Tick(DeltaTime, GetWorld());

	/*
	if(hipIsPivot){
		hipController.Tick(DeltaTime, GetWorld());
		
		//M = Tshoulder * Thip * Rhip  <-- lese richtung --
		MMatrix hip = hipController.currentTransform();
		MMatrix applied = hipToShoulderMatrix * hip;
		FVector offsetVector = applied.getTranslation();

		shoulderController.Tick(DeltaTime, GetWorld(), offsetVector);
		
	}else{
		
		shoulderController.Tick(DeltaTime, GetWorld());
		MMatrix shoulder = shoulderController.currentTransform();

		MMatrix relationInversed = hipToShoulderMatrix.createInverse();
		MMatrix applied = relationInversed * shoulder; //M = T * A <-- lese richtung --
		hipController.Tick(DeltaTime, GetWorld(), applied.getTranslation());

	}
	*/
	
}




/// @brief look at a location
/// @param TargetLocation target to look at
void AIkActor::LookAt(FVector TargetLocation) 
{
    // Calculate the rotation needed to look at the target location
	MMatrix transform = currentTransform();
	FVector location = transform.getTranslation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(location, TargetLocation);

	//FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	float zDegree = LookAtRotation.Yaw;

	ownOrientation.yawRad(MMatrix::degToRadian(zDegree));
}




/// @brief returns the current translation and rotation in world in correct order as expected
/// HIP PIVOT TRANSFORM ONLY
/// @return current Transform Matrix by value
MMatrix AIkActor::currentTransform(){ //might be renamed to hip pivot 
	MMatrix rotationTransform = ownLocation * ownOrientation;
	return rotationTransform;
}



//custom set actor location method for bone ik
void AIkActor::SetLocation(FVector &location){
	ownLocation.setTranslation(location);
}







/// @brief converts a position to relative to the hip / actor center
/// @param position position to convert, will be adjusted by reference
void AIkActor::transformFromWorldToLocalCoordinates(FVector &position){

	MMatrix inverse = currentTransform().createInverse();

	position = inverse * position;
}

/// @brief performs a raycast from a start, into a direction, with a max length in this method
/// @param Start start position
/// @param dir direction of interest
/// @param outputHit output hit if any hit happened
/// @return bool anything was hit
bool AIkActor::performRaycast(FVector &Start, FVector &dir, FVector &outputHit) 
{

	float scaleOfVector = legScaleCM * 3; //some random value for now
	FVector End = Start + dir * scaleOfVector; // gx = A + r (B - A)

	// Perform the raycast
	FHitResult HitResult;

	FCollisionQueryParams ignoreParams;
	/*
	CAUTION: TEAMS PARAM NOT IMPLEMETED YET!

	//params to avoid hitting any other entities
	if(EntityManager *e = worldLevel::entityManager()){
		//ignoreParams = e->getIgnoredRaycastParams(); //example for getting all
		ignoreParams = e->getIgnoredRaycastParams(getTeam());
	}*/
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	



	//anything was hit
	if (bHit){
		outputHit = HitResult.ImpactPoint; //write impactpoint to output

		
		//debug draw
		if(debugRaycastDraw){
			DebugHelper::showLineBetween(
				GetWorld(),
				currentTransform().getTranslation(),
				outputHit,
				FColor::Orange, 
				2.0f
			);
		}
		
		//DebugHelper::showScreenMessage("RAYCAST HIT");

		return true;
	}

	return false;
}











void AIkActor::projectToGround(FVector &frameToProject){
	MMatrix transform = currentTransform();
	FVector frameInWorld = transform * frameToProject;

	//project frame to floor
	FVector downVec(0, 0, -1);
	FVector hitpoint;


	bool wasHit = performRaycast(frameInWorld, downVec, hitpoint);
	if(wasHit){
		transformFromWorldToLocalCoordinates(hitpoint);
		frameToProject = hitpoint;
	}
}




/// @brief projects a frame to the ground and writes the porjection into the frame
/// @param frameToProject frame to project to floor
/// @param offsetMade offset made from frame to project to hitpoint direction
void AIkActor::projectToGround(FVector &frameToProject, FVector &offsetMade){
	MMatrix transform = currentTransform();
	FVector frameInWorld = transform * frameToProject;

	//EXTRA OFFSET NEEDED HERE, sont terrain nicht berührt, könnte durchlaufen!
	frameInWorld += FVector(0, 0, legScaleCM); //TEST RAYCAST START NACH OBEN!

	//project frame to floor
	FVector downVec(0, 0, -1);
	FVector hitpoint;

	bool wasHit = performRaycast(frameInWorld, downVec, hitpoint);
	if(wasHit){
		//offsetMade = hitpoint - frameInWorld; // AB = B - A;
		if(debugRaycastDraw){
			DebugHelper::showLineBetween(
				GetWorld(), 
				transform.getTranslation(), 
				hitpoint, 
				FColor::Red, 
				2.0f
			);
		}
		

		transformFromWorldToLocalCoordinates(hitpoint);

		//DebugHelper::showScreenMessage("projected ", hitpoint, FVector(), FColor::Orange);

		offsetMade = hitpoint - frameToProject;

		frameToProject = hitpoint;
	}
}






/**
 * 
 * 
 * ---- new section for limb creation ----
 * 
 * 
 */

AActor *AIkActor::createLimbPivotAtTop(int x, int y, int height, int offsetY){

	height *= -1; //orient downwardss
	/**
	 * DEBUG CREATE FOLLOW LIMBS
	 */
	UMaterial *material = nullptr;
	assetManager *assetManagerPointer = assetManager::instance();
	if(assetManagerPointer != nullptr){
		material = assetManagerPointer->findMaterial(materialEnum::wallMaterial);
	}

	EntityManager *entitymanagerPointer = worldLevel::entityManager();
	if(entitymanagerPointer != nullptr){
		FVector location(0, 0, 0);
		AcustomMeshActor *oberschenkel = entitymanagerPointer->spawnAcustomMeshActor(GetWorld(), location);
		if(oberschenkel != nullptr){
			//int width = 10;
			//int height = -(legScaleCM / 2);

			FVector a(0,offsetY,0);
			FVector b(x,offsetY,0);
			FVector c(x,y + offsetY,0);
			FVector d(0, y + offsetY,0);
			FVector at(0,offsetY,height);
			FVector bt(x,offsetY,height);
			FVector ct(x,y + offsetY,height);
			FVector dt(0,y + offsetY,height);
			oberschenkel->createCube(
				a,b,c,d,at,bt,ct,dt,
				material
			);
			return oberschenkel;
		}
	}
	return nullptr;
}




