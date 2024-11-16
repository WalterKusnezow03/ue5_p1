// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/BoneIk.h"
#include "Kismet/KismetMathLibrary.h"
#include <cmath>
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"
#include "p2/entities/customIk/animation/DoubleKeyFrameAnimation.h"
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
	FVector offset(1000, -1000, 200);
	ownLocation.setTranslation(offset);
	offset -= FVector(0, 0, legScaleCM);
	ownLocationFoot.setTranslation(offset);

	
	leg1.setupBones(legScaleCM);
	leg2.setupBones(legScaleCM);
	leg2.setDegreeInital(180); //testing for offset movement

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
	animationKeys_1.addFrame(FVector(100, 100, 100), 0.1f);

	//testing
	//ownOrientation.yawRadAdd(MMatrix::degToRadian(90));

	//testing look at, works
	//FVector target(0, 0, 0);
	//LookAt(target);

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

	//NEU mit double kette
	KeyFrameAnimation legHipAdjustKeys; 
	legHipAdjustKeys.addFrame(
		FVector(0, 0, 0),
		0.001f,
		false,
		legScaleCM
	);
	legHipAdjustKeys.addFrame(
		FVector(0, 0, 200),
		1.0f,
		false,
		legScaleCM
	);

	//NEEDS TO BE TESTED
	legDoubleKeys_1.setAnimationA(MoveTemp(legAnimationKeys));
	legDoubleKeys_1.setAnimationB(MoveTemp(legHipAdjustKeys));
}

// Called every frame
void AIkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//neu braucht testing!
	MMatrix orientationLocation = currentTransform(); 
	
	MMatrix armMat = orientationLocation;
	FVector verticalOffset(0, 0, 100);
	armMat += verticalOffset;

	//orientation rotation matrix dann reingeben als first node / limb 
	arm1.build(GetWorld(), armMat, FColor::Purple, DeltaTime * 2);
	

	//debug draw of targets
	//arms new, must be added
	FVector armOff = orientationLocation.getTranslation() + FVector(0, 0, 100); // up offset for arms
	
	FVector t1 = targetA;
	FVector t2 = targetB;
	t1 += armOff;
	t2 += armOff;
	//DebugHelper::showLineBetween(GetWorld(), t1, t2, FColor::Green, DeltaTime * 2); //draw line of arm movement
	
	
	//standAloneKeyFrameAnimAndHipAdjustTime(leg1, legAnimationKeys, DeltaTime);
	KeyFrameAnimAndHipAdjustTime(leg1, legDoubleKeys_1, DeltaTime);

	standAloneKeyFrameAnim(arm1, animationKeys_1, DeltaTime);
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



















//THIS METHOD IS ONLY FOR FOOT FOR NOW
void AIkActor::standAloneKeyFrameAnim(BoneIk &bone, KeyFrameAnimation &frames, float DeltaTime){
	//plotNextFrameToGround(frames); //TESTING NEEDED, must be only for leg!

	//erstmal nur end to target!
	FVector weight(-1, 0, 0);

	FVector nextPos = frames.interpolate(DeltaTime); //new function
	MMatrix translationActor = currentTransform();
	
	//dummy adjust, not hip
	MMatrix dummy;

	bone.rotateEndToTargetAndBuild(
		GetWorld(),
		nextPos,
		weight,
		translationActor, //offset and rotation
		dummy,
		FColor::Blue,
		DeltaTime * 2.0f
	);
}

void AIkActor::standAloneKeyFrameAnimProjected(
	BoneIk &bone, KeyFrameAnimation &frames, float DeltaTime
){
	plotNextFrameToGround(frames); //testing was correct 

	//erstmal nur end to target!
	FVector weight(1, 0, 0);
	FVector nextPos = frames.interpolate(DeltaTime); //new function
	MMatrix translationActor = currentTransform();
	
	bone.rotateEndToTargetAndBuild(
		GetWorld(),
		nextPos,
		weight,
		translationActor, // hip start with orient
		ownLocationFoot,  // foot apply position
		ownLocation, // hip apply position
		FColor::Black,
		DeltaTime * 2.0f
	);
}









/**
 * 
 * NEW LEG SWITCH WITH ANIM
 * 
 */


//CAUTION WILL ONLY WORK FOR ONE, NEEDS PHASE BLOCKING CLASS OR SOMTHING LIKE THAT!
//TOuPLE WITH EACH BONE IDK! SOEMTHING LIKE THAT NEEDED!
void AIkActor::standAloneKeyFrameAnimAndHipAdjustTime(BoneIk &bone, KeyFrameAnimation &frames, float DeltaTime){
	//hier end und start!
	FVector weight(1, 0, 0);

	//TESTING NEEDED!
	float halfTime = frames.totalLength();

	float fullTime = halfTime * 2.0f;
	debugFlipTime += DeltaTime;
	bool moveLeg = debugFlipTime < halfTime;
	if (debugFlipTime >= fullTime){
		debugFlipTime = 0; //otherwise causes issues with leg glitching / hip !, must be fixed later
		return;
	}
	//DebugHelper::showScreenMessage("time now ", debugFlipTime);


	if(moveLeg){
		standAloneKeyFrameAnimProjected(bone, frames, DeltaTime);
	}else{

		FVector footTarget = frames.readPrevFrame();
		FVector hipStart = hipRelativeToFootRelativeTarget(footTarget);


		float time = debugFlipTime - halfTime;
		float skalar = time / halfTime; //x / 1
		
		// HIP TO TARGET
		standAloneMoveStartFromTo(
			bone,
			hipStart, // start
			FVector(0, 0, 200), // target original pos
			DeltaTime,
			skalar // irgendwas / 1 = 0.irgendwas
		);
	}
}

/// @brief moves the start (hip to a wanted target) relative to the bone end limb
/// @param bone 
/// @param start 
/// @param target 
/// @param DeltaTime 
/// @param skalar 
void AIkActor::standAloneMoveStartFromTo(
	BoneIk &bone, 
	FVector start, 
	FVector target,
	float DeltaTime,
	float skalar //custom time progress
){

	FVector weight(1, 0, 0);
	FVector _direction = target - start;
	FVector xt = start + _direction * skalar;


	MMatrix current_translationActorFoot = currentFootTransform();

	bone.rotateStartToTargetAndBuild(
		GetWorld(),
		xt,
		weight,
		current_translationActorFoot, // foot start
		ownLocation, // hip apply
		FColor::Black,
		DeltaTime * 2.0f
	);

}








/// @brief converts a hip relative target to a foot relative target for the hip
/// @param other 
/// @return 
FVector AIkActor::hipRelativeToFootRelativeTarget(FVector &other){
	//FVector footTarget(50, 0, -150); // relative to hip
	//standAloneMoveStartFromTo(bone, FVector(-50, 0, 150), FVector(0, 0, 200), DeltaTime); //reltaive to foot

	return other * -1;
}

FVector AIkActor::worldToHipRelativeTarget(FVector &other){
	MMatrix currentTransformMatrix = currentTransform();
	return other - currentTransformMatrix.getTranslation();
}

/// @brief returns the current translation and rotation in world in correct order as expected
/// HIP PIVOT TRANSFORM ONLY
/// @return current Transform Matrix by value
MMatrix AIkActor::currentTransform(){ //might be renamed to hip pivot 
	// TR = R * T;
	// RT = T * R;
	//gewünscht ist jetzt erst an end punkt und dann um eigene
	//achse zu rotieren, also (geprüft!):
	//MMatrix rotationTransform = ownLocation * ownOrientation;

	MMatrix rotationTransform = ownLocation;
	rotationTransform *= ownOrientation;
	return rotationTransform;
}

/// @brief returns the current translation and rotation in world in correct order as expected
/// LOWER LEG PIVOT TRANSFORM ONLY, USE FOR TARGET ADJUST HIP! "rotateStartToTargetAndBuild()" FUNCTION OF BONEIK
/// @return current Transform Matrix by value
MMatrix AIkActor::currentFootTransform(){
	//RT = T * R
	MMatrix rotationTransform = ownLocationFoot * ownOrientation;
	return rotationTransform;
}

//custom set actor location method for bone ik
void AIkActor::SetLocation(FVector &location){

	//custom for bones
	FVector copy = ownLocation.getTranslation();
	FVector dirToFoot = ownLocationFoot.getTranslation() - copy; // AB = B - A

	ownLocation.setTranslation(location);

	FVector locationForFoot = location + dirToFoot;
	ownLocationFoot.setTranslation(locationForFoot);

}








void AIkActor::transformFromWorldToLocalCoordinates(FVector &position){
	MMatrix inverted = currentTransform();
	inverted.invert(); //damit ich den punkt von world in local bringe

	//remove rool and pitch component, only keep yaw
	//inverted.rollRad(0);
	//inverted.pitchRad(0);

	FVector toLocal = inverted * position; 
	MMatrix current = currentTransform();
	FVector inWorld = current * toLocal;
	DebugHelper::showLineBetween(GetWorld(), inWorld, position, FColor::Cyan);
	DebugHelper::showLineBetween(GetWorld(), inWorld, inWorld + FVector(100,0,0), FColor::Cyan);
	position = toLocal;
}


// TESTING NEEDED
void AIkActor::plotNextFrameToGround(KeyFrameAnimation &animation){
	//if not grounded, dont
	if(!animation.nextFrameMustBeGrounded()){
		return;
	}
	if(animation.nextFrameIsProjected()){
		//DebugHelper::showScreenMessage("was already projected ", FColor::Cyan);
		return;
	}

	//DebugHelper::showScreenMessage("try plot ", FColor::Red);
	FVector frameToProject = animation.readNextFrame();
	MMatrix transform = currentTransform();
	FVector frameInWorld = transform * frameToProject;

	//project frame to floor
	FVector downVec(0, 0, -1);
	FVector hitpoint;


	bool wasHit = performRaycast(frameInWorld, downVec, hitpoint);
	if(wasHit){
		transformFromWorldToLocalCoordinates(hitpoint);
		animation.overrideNextFrame(hitpoint); //100% NÖTIG! PROJECT TARGET TO GROUND!
	}
	
}


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
		DebugHelper::showLineBetween(
			GetWorld(),
			currentTransform().getTranslation(),
			outputHit,
			FColor::Orange, 
			2.0f
		);
		//DebugHelper::showScreenMessage("RAYCAST HIT");

		return true;
	}

	return false;
}





/**
 * 
 * NEW WAIT TIME SECTION!
 * 
 */


// NEW TESTING WITH DOUBLE FRAMES!

void AIkActor::KeyFrameAnimAndHipAdjustTime(BoneIk &bone, DoubleKeyFrameAnimation &frames, float DeltaTime){
	
	bool moveLeg = frames.isAnimationA();

	if(moveLeg){
		standAloneKeyFrameAnim(bone, frames, DeltaTime);	
	}else{
		

		FVector weight(1, 0, 0);

		if(!frames.currentAndNextForBOverriden()){
			FVector footTarget = frames.readPrevAnimationReachedFrame();
			FVector hipStart = hipRelativeToFootRelativeTarget(footTarget);

			MMatrix currentAct = currentFootTransform();
			FVector plot = currentAct * hipStart;
			DebugHelper::showLineBetween(
				GetWorld(), 
				currentAct.getTranslation(), 
				plot, 
				FColor::Green, 
				5.0f
			);

			FVector hipTarget(0, 0, 200);
			float timeWantedSmaple = 1.0f; //needs to be changed later
			frames.tryOverrideCurrentAndNextFrameAnimB(hipStart, hipTarget, timeWantedSmaple);
		}
		

		FVector xt = frames.interpolate(DeltaTime);

		MMatrix current_translationActorFoot = currentFootTransform();
		bone.rotateStartToTargetAndBuild(
			GetWorld(),
			xt,
			weight,
			current_translationActorFoot, // foot start
			ownLocation, // hip apply
			FColor::Blue,
			DeltaTime * 2.0f
		);

	}
}




//new leg update mit double frame animation
void AIkActor::standAloneKeyFrameAnim(
	BoneIk &bone, DoubleKeyFrameAnimation &frames, float DeltaTime
){
	bool mustBeGrounded = frames.nextFrameMustBeGrounded();
	bool isalreadyProjected = frames.nextFrameIsProjected();

	if(mustBeGrounded && !isalreadyProjected){
		FVector nextFramePos = frames.readNextFrame();

		//neu offset speichern in double frame anim
		FVector projectOffsetMade(0,0,0);
		projectToGround(nextFramePos, projectOffsetMade);
		frames.processProjectOffset(projectOffsetMade);

		//projectToGround(nextFramePos); //old, replaced with offset save too.
		frames.overrideNextFrame(nextFramePos);
	}

	//HIP PROJECTION ADJUST DO NOT REMOVE!
	MMatrix translationActorTmp = currentTransform();
	FVector hipoffsetAdd = frames.getProjectionHipOffsetTimed();
	DebugHelper::showLineBetween(
			GetWorld(), 
			translationActorTmp * hipoffsetAdd, 
			translationActorTmp.getTranslation(), 
			FColor::Red, 
			2.0f
	);
	ownLocation += hipoffsetAdd; //intern matrix



	//do movement	
	FVector weight(1, 0, 0);
	FVector nextPos = frames.interpolate(DeltaTime);
	MMatrix translationActor = currentTransform();

	bone.rotateEndToTargetAndBuild(
		GetWorld(),
		nextPos,
		weight,
		translationActor, // hip start with orient
		ownLocationFoot,  // foot apply position
		//ownLocation, // hip apply position -----> adjustment is made HERE ABOVE!
		FColor::Black,
		DeltaTime * 2.0f
	);
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




/**
 * 
 * New method which also will return the offset as vector
 * which should be devided by the time to anim end
 * to adjust the hip linearly
 * (Maybe saved in double key frame animation and resettet once animation is complete / reached)
 * or returns it as hip extra adjust / end extra adjust
 * ----> could get the offset alone and do the calculation with animation time of framesA 
 * 		 on its own!
 * 
 * 		-> todo: 
 * 		   - get offset as vector
 * 		   - function for double frame animation -> inputProjectOffset(FVector) -> copied to Instance Var
 * 		   - > this function devides it and resets when framesA is done!
 */
void AIkActor::projectToGround(FVector &frameToProject, FVector &offsetMade){
	MMatrix transform = currentTransform();
	FVector frameInWorld = transform * frameToProject;

	//project frame to floor
	FVector downVec(0, 0, -1);
	FVector hitpoint;

	bool wasHit = performRaycast(frameInWorld, downVec, hitpoint);
	if(wasHit){
		//offsetMade = hitpoint - frameInWorld; // AB = B - A;
		transformFromWorldToLocalCoordinates(hitpoint);

		offsetMade = hitpoint - frameToProject;
		DebugHelper::showLineBetween(
			GetWorld(), 
			transform * offsetMade, 
			transform.getTranslation(), 
			FColor::Red, 
			2.0f
		);

		frameToProject = hitpoint;
	}
}