// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/BoneIk.h"
#include "Kismet/KismetMathLibrary.h"
#include <cmath>
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"
#include "p2/entities/customIk/animation/DoubleKeyFrameAnimation.h"
#include "p2/gameStart/assetManager.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entities/customIk/IkActor.h"

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
	FVector offset(1000, -1000, legScaleCM);
	ownLocation.setTranslation(offset); //hip location initial
	offset -= FVector(0, 0, legScaleCM);
	
	//make foot
	ownLocationFoot1.setTranslation(offset);
	ownLocationFoot2.setTranslation(offset);

	//make chest
	FVector copy = FVector(0, 0, legScaleCM / 2);
	chestRelative.setTranslation(copy);

	leg1.setupBones(legScaleCM);
	leg2.setupBones(legScaleCM);
	//leg2.setDegreeInital(180); //testing for offset movement

	float armScale = legScaleCM * 0.8f;
	arm1.setupBones(armScale);
	
	


	//x ist forward
	FVector target2(1.5f, -0.5f, 0); //sollte das selbe ergebnis prodzuieren bis jetzt: ja tut es.
	FVector weight(1, 1, -1);
	arm1.rotateEndToTarget(target2, weight); //testing notwendig
	
	//testing values for arm movement
	targetA = FVector(1.0f, -0.5f, 2); //z 0  
	targetB = FVector(1.5f, 1.0f, -1);
	targetB = targetA - FVector(0, 0, 1); //testing

	targetA *= 100; //auf cm skalieren.
	targetB *= 100;

	animationKeys_1.addFrame(targetA, 0.1f);
	animationKeys_1.addFrame(targetB, 1.0f);
	animationKeys_1.addFrame(FVector(100, 100, 100), 2.0f);

	//testing inital rotation
	int DEBUGROATATE = 0;
	ownOrientation.yawRad(MMatrix::degToRadian(DEBUGROATATE));

	//legTarget = FVector(100, 0, -100);

	//muss nur forward motion eigentlich haben
	legAnimationKeys = KeyFrameAnimation(false); //instant flip animation
	legAnimationKeys.addFrame(
		FVector(0, 0, -200),
		0.0f,
		false,
		legScaleCM // behebt viele fehler!
	);
	legAnimationKeys.addFrame(
		FVector(25, 0, -190),
		0.5f,
		false,
		legScaleCM
	);
	legAnimationKeys.addFrame(
		FVector(50, 0, -200),
		0.5f,
		true,
		legScaleCM
	);

	KeyFrameAnimation legKeysCopy = legAnimationKeys;

	legDoubleKeys_1.setAnimationA(MoveTemp(legAnimationKeys));
	legDoubleKeys_2.setAnimationA(MoveTemp(legKeysCopy));

	FVector bTarget = FVector(0, 0, legScaleCM);
	legDoubleKeys_1.setAnimationBAdjustPermanentTarget(bTarget);
	legDoubleKeys_2.setAnimationBAdjustPermanentTarget(bTarget);



	legDoubleKeys_1.setRunning(true);
	legDoubleKeys_2.setRunning(true);

	// legDoubleKeys_1.setAnimationB(MoveTemp(legHipAdjustKeys)); //DEPRECATED

	/*
	//CREATE LIMBS
	AActor *oberschenkelLimb = createLimbPivotAtTop(10, 10, legScaleCM / 2, -20);
	leg1.attachFirtsLimb(*oberschenkelLimb);
	AActor *unterschenkelLimb = createLimbPivotAtTop(10, 10, legScaleCM / 2, -20);
	leg1.attachSecondLimb(*unterschenkelLimb);
	AActor *footLimb = createLimbPivotAtTop(30, 10, 10, -20);
	//leg1.attachThirdLimb(*footLimb);



	AActor *oberschenkelLimb1 = createLimbPivotAtTop(10, 10, legScaleCM / 2, 20);
	leg2.attachFirtsLimb(*oberschenkelLimb1);
	AActor *unterschenkelLimb1 = createLimbPivotAtTop(10, 10, legScaleCM / 2, 20);
	leg2.attachSecondLimb(*unterschenkelLimb1);
	AActor *footLimb1 = createLimbPivotAtTop(30, 10, 10, 20);
	//leg2.attachThirdLimb(*footLimb1);
	*/


	//debug matrix inverse bestimmen:
	MMatrix current = currentTransform();
	MMatrix invers = current.jordanInverse(); // current.adjunktInverse();
	MMatrix identitity = current * invers;

	FString message1 = FString::Printf(TEXT("debug matrix inverse matrix: "));
	message1 += invers.asString();
	DebugHelper::logMessage(message1);

	FString message = FString::Printf(TEXT("debug matrix Sollte einheits matrix ergeben: "));
	message += identitity.asString();
	DebugHelper::logMessage(message);



	//DebugHelper::logMessage(current.asString());
	//DebugHelper::logMessage(invers.asString());

}

// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//neu braucht testing!
	MMatrix orientationLocation = currentTransform(); 
	
	

	MMatrix armMat = currentShoulderTransform();
	//standAloneKeyFrameAnim(arm1, animationKeys_1, armMat, DeltaTime, FColor::Purple);



	//DEBUG TESTING

	FVector pos = ownLocationFoot1.getTranslation();
	currentTransform().transformFromWorldToLocalCoordinates(pos);
	//DebugHelper::showScreenMessage("foot 1 pos (Y must be 0 always) ", pos, FColor::Orange);



	

	FColor leg1Color = FColor::Red;
	FColor leg2Color = FColor::Blue;

	//KeyFrameAnimAndHipAdjustTime(leg1, legDoubleKeys_1, ownLocationFoot1, DeltaTime, leg1Color);
	//return;

	if (leg1isPlaying)
	{
		
		buildRawAndKeepEndInPlace(leg2, ownLocationFoot2, DeltaTime, leg2Color);
		KeyFrameAnimAndHipAdjustTime(leg1, legDoubleKeys_1, ownLocationFoot1, DeltaTime, leg1Color);

		//switch to next leg but update relative position
		if(legDoubleKeys_1.animationCycleWasComplete()){
			leg1isPlaying = false;

			//die hip wurde mit dem anderen bein bewegt /(leg 1, also muss leg2 geupdated werden)
			FVector footPos = ownLocationFoot2.getTranslation();
			DebugHelper::showLineBetween(GetWorld(), footPos, currentTransform().getTranslation(), FColor::Orange, 2.0f);
			transformFromWorldToLocalCoordinates(footPos);
			legDoubleKeys_2.tryPushFront(footPos); //update starting pos

		}
		

	}else{
		
		buildRawAndKeepEndInPlace(leg1, ownLocationFoot1, DeltaTime, leg1Color);
		KeyFrameAnimAndHipAdjustTime(leg2, legDoubleKeys_2, ownLocationFoot2, DeltaTime, leg2Color);

		if(legDoubleKeys_2.animationCycleWasComplete()){//left leg done
			leg1isPlaying = true;

			//die hip wurde mit dem anderen bein bewegt /(leg 2, also muss leg1 geupdated werden)
			FVector footPos = ownLocationFoot1.getTranslation();
			DebugHelper::showLineBetween(GetWorld(), footPos, currentTransform().getTranslation(), FColor::Orange, 2.0f);
			transformFromWorldToLocalCoordinates(footPos);
			legDoubleKeys_1.tryPushFront(footPos); //update starting pos
			
		}
	}
	
	
}



/// @brief renders the leg raw if the hip is moved!
/// @param boneIk bone to render which foot stays in place
/// @param legTransform leg position to hold and not move from
/// @param deltaTime delta time
/// @param color color to render
void AIkActor::buildRawAndKeepEndInPlace(BoneIk &boneIk, MMatrix &legTransform, float deltaTime, FColor color){
	/*
	build leg raw

	ziel hier: 
	von hier das bein so rendern das bein am originalen punkt bleibt
	das foot transform muss also zu einem target umgewandelt werden
	und dann dort hin bewegt. Egal ob was neu berechnet wird oder nicht
	*/
	
	

	FVector weight(1, 0, 0); // gewicht zieht nach vorne

	MMatrix currenttransform = currentTransform();
	FVector pos = legTransform.getTranslation();
	currenttransform.transformFromWorldToLocalCoordinates(pos); //relativ zur hüfte neu berechnen, auchwenn hüfte sich bewegt!

	boneIk.rotateEndToTargetAndBuild(
		GetWorld(),
		pos,
		weight,
		currenttransform, // MMatrix &offsetAndRotation,
		legTransform,
		color,//FColor::Red,
		deltaTime * 2.0f // displayTime
	);
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











/**
 * PLAY ANIM FOR ANY BONE, ONLY FORWARD IK
 */

/// @brief plays an animation for any bone but will not the actor location
/// @param bone bone to play for
/// @param frames frames to play
/// @param DeltaTime delta time from Tick
void AIkActor::standAloneKeyFrameAnim(BoneIk &bone, KeyFrameAnimation &frames, float DeltaTime){

	MMatrix translationActor = currentTransform();

	standAloneKeyFrameAnim(
		bone,
		frames,
		translationActor,
		DeltaTime,
		FColor::Blue
	);
}

/**
 * soll den knochen bauen anhand animation und einen offset vorgeben
 */
void AIkActor::standAloneKeyFrameAnim(
	BoneIk &bone, 
	KeyFrameAnimation &frames, 
	MMatrix &initalTransform, 
	float DeltaTime, 
	FColor color
){
	//erstmal nur end to target!
	FVector weight(-1, 1, 0); //testing needed for arm rotation

	FVector nextPos = frames.interpolate(DeltaTime); //new function
	//MMatrix translationActor = currentTransform();
	
	//dummy adjust, not foot or end, nothing for now. Could be hand.
	MMatrix dummy;

	bone.rotateEndToTargetAndBuild(
		GetWorld(),
		nextPos,
		weight,
		initalTransform, //offset and rotation
		dummy,
		color,
		DeltaTime * 2.0f
	);
}














/// @brief converts a hip relative target to a foot relative target for the hip
/// @param other 
/// @return 
FVector AIkActor::hipRelativeToFootRelativeTarget(FVector &other){
	return other * -1;
}



/// @brief returns the current translation and rotation in world in correct order as expected
/// HIP PIVOT TRANSFORM ONLY
/// @return current Transform Matrix by value
MMatrix AIkActor::currentTransform(){ //might be renamed to hip pivot 
	MMatrix rotationTransform = ownLocation * ownOrientation;
	return rotationTransform;
}



MMatrix AIkActor::currentFootTransform(MMatrix &foottranslationToRotate){
	//M = T * R <-- lese richtung --
	MMatrix rotationTransform = foottranslationToRotate * ownOrientation; //rotiert fuss das es +bereinstimmt mit actor
	return rotationTransform;
}


MMatrix AIkActor::currentShoulderTransform(){
	MMatrix currentT = currentTransform();
	MMatrix shoulderTransform = chestRelative * currentT; //<-- lese reichtung --
	return shoulderTransform;
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
 * NEW SECTION FOR SEPERATE FOOT MATRICIES WHICH IS NEEDED FOR INDIVIDUAL MOVEMENT!
 * 
 * ONLY FOOT SECTION
 * 
 * 
 */


void AIkActor::KeyFrameAnimAndHipAdjustTime(
	BoneIk &bone, 
	DoubleKeyFrameAnimation &frames, 
	MMatrix &footMatrix, //MMatrix foot transform 
	float DeltaTime,
	FColor color
){
	
	bool moveLeg = frames.isAnimationA();

	if(moveLeg){
		standAloneKeyFrameAnim(bone, frames, footMatrix, DeltaTime, color);	
	}else{
		//HIP ADJUST 

		FVector weight(1, 0, 0);

		//offset for foot during hip adjust movement
		FVector hipoffsetAdd = frames.getProjectionHipOffsetTimed();
		footMatrix += hipoffsetAdd;

		//build
		FVector xt = frames.interpolate(DeltaTime);




		MMatrix current_translationActorFoot = currentFootTransform(footMatrix);
		bone.rotateStartToTargetAndBuild(
			GetWorld(),
			xt,
			weight,
			current_translationActorFoot, // foot start
			ownLocation, // hip apply
			color, //FColor::Blue,
			DeltaTime * 2.0f
		);



		MMatrix m = currentFootTransform(footMatrix);
		FVector worldSpace = m * xt;

		//debug draw
		//DebugHelper::showLineBetween(GetWorld(), m.getTranslation(), worldSpace, FColor::Emerald, 2.0f);


	}
}


void AIkActor::standAloneKeyFrameAnim(
	BoneIk &bone, 
	DoubleKeyFrameAnimation &frames, 
	MMatrix &footMatrix,
	float DeltaTime,
	FColor color
){

	MMatrix current = currentTransform();
	//frames.projectNextFrameIfNeeded(GetWorld(), current); //default projektion

	bool isRunning = true;
	if(!isRunning){
		frames.projectNextFrameIfNeeded(GetWorld(), current);
	}else{
		float velocityT = 200.0f; //2ms
		FVector lookDir = currentTransform().lookDirXForward();
		lookDir.Z = 0.0f; //xy pane only of interest
		FVector thisAdd = lookDir * velocityT * DeltaTime;

		frames.projectNextFrameIfNeeded(GetWorld(), current, velocityT, lookDir);

		ownLocation += thisAdd; //running velocity
	}

	/**
	 * HIP PROJECTION ADJUST DO NOT REMOVE! 
	 * Adds the needed hip offset over time
	 * for the hip when the point gets projected to the floor
	*/
	MMatrix translationActorTmp = currentTransform();
	FVector hipoffsetAdd = frames.getProjectionHipOffsetTimed();
	ownLocation += hipoffsetAdd; 
	DebugHelper::showLineBetween(
			GetWorld(), 
			translationActorTmp * hipoffsetAdd, 
			translationActorTmp.getTranslation(), 
			FColor::Red, 
			2.0f
	);


	//do movement	
	FVector weight(1, 0, 0);
	FVector nextPos = frames.interpolate(DeltaTime);
	MMatrix translationActor = currentTransform();

	//DebugHelper::showScreenMessage("interpolated ", nextPos, FColor::Red);

	bone.rotateEndToTargetAndBuild(
		GetWorld(),
		nextPos,
		weight,
		translationActor, // hip start with orient
		footMatrix, //ownLocationFoot,  // foot apply positions
		color,//FColor::Black,
		DeltaTime * 2.0f
	);
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




