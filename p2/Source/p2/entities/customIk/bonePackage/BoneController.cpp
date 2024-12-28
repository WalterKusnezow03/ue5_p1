// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/bonePackage/BoneController.h"
#include "p2/entities/customIk/bonePackage/TwoBone.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"
#include "p2/entities/customIk/MMatrix.h"
#include "Kismet/KismetMathLibrary.h"
#include "p2/entities/customIk/bonePackage/BoneControllerStates.h"
#include "CoreMinimal.h"

BoneController::BoneController()
{
	attachedCarriedItem = nullptr;
	setupBones();
	setupAnimation(); // temporary zum denken

	currentMotionState = BoneControllerStates::none; //by default

}

BoneController::BoneController(float legScaleCmIn, float armScaleCmIn){
	attachedCarriedItem = nullptr;
	legScaleCM = std::abs(legScaleCmIn);
	armScaleCM = std::abs(armScaleCmIn);
	setupBones();
	setupAnimation();
	currentMotionState = BoneControllerStates::none;
}

BoneController::BoneController(BoneController &other){
	attachedCarriedItem = nullptr;
	if(&other != this){
		*this = other;
	}
}

BoneController &BoneController::operator=(BoneController &other){
	if(&other == this){
		return *this;
	}

	ownLocation = other.ownLocation;
	ownOrientation = other.ownOrientation;

	ownLocationFoot1 = other.ownLocationFoot1; //real world location end foot 1
	ownLocationFoot2 = other.ownLocationFoot2; //real world location end foot 2

	ownLocationHand1 = other.ownLocationHand1;
	ownLocationHand2 = other.ownLocationHand2;

	hip1MatrixOffset = other.hip1MatrixOffset; //local offset to hip com
	hip2MatrixOffset = other.hip2MatrixOffset; //local offset to hip com

	shoulder1MatrixOffset = other.shoulder1MatrixOffset;
	shoulder2MatrixOffset = other.shoulder2MatrixOffset;

	legDoubleKeys_1 = other.legDoubleKeys_1;
	legDoubleKeys_2 = other.legDoubleKeys_2;

	legScaleCM = other.legScaleCM;

	leg1 = other.leg1;
	leg2 = other.leg2;

	arm1 = other.arm1;
	arm2 = other.arm2;

	armClimbKeys_1 = other.armClimbKeys_1;

	currentMotionState = other.currentMotionState;
	currentArmMotionState = other.currentArmMotionState;

	return *this;
}

BoneController::~BoneController()
{

}



/// @brief finds the bone by index, returns a pointer to the bone
/// @param limbIndex 
/// @return 
TwoBone *BoneController::findBone(int limbIndex){
	if(limbIndex == FOOT_1){
		return &leg1;
	}
	if(limbIndex == FOOT_2){
		return &leg2;
	}
	if(limbIndex == SHOULDER_1){
		return &arm1;
	}
	if(limbIndex == SHOULDER_2){
		return &arm2;
	}

	return nullptr;
}

/// @brief finds the end effector by index, returns a pointer to the endeffector
/// @param limbIndex 
/// @return 
MMatrix *BoneController::findEndEffector(int limbIndex){
	if(limbIndex == FOOT_1){
		return &ownLocationFoot1;
	}
	if(limbIndex == FOOT_2){
		return &ownLocationFoot2;
	}
	if(limbIndex == SHOULDER_1){
		return &ownLocationHand1;
	}
	if(limbIndex == SHOULDER_2){
		return &ownLocationHand2;
	}

	return nullptr;
}



FColor BoneController::limbColor(int limb){
	if(limb == FOOT_1){
		return leg1Color;
	}
	if(limb == FOOT_2){
		return leg2Color;
	}
	if(limb == SHOULDER_1){
		return arm1Color;
	}
	if(limb == SHOULDER_2){
		return arm2Color;
	}
	return FColor::Black;
}



//construct bones
void BoneController::setupBones(){
	//setup
    leg1.setupBones(legScaleCM);
	leg2.setupBones(legScaleCM);

	arm1.setupBones(armScaleCM);
	arm2.setupBones(armScaleCM);

	int hipSideoffset = 10;
	int shoulderSideOffset = 15;

	//X axis is forward
    FVector sideLeft(0, -hipSideoffset, 0);
    FVector sideRight(0, hipSideoffset, 0);
    hip1MatrixOffset.setTranslation(sideLeft);
    hip2MatrixOffset.setTranslation(sideRight);



	FVector sideLeftShoulder(0, -shoulderSideOffset, armScaleCM);
	FVector sideRightShoulder(0, shoulderSideOffset, armScaleCM);
	shoulder1MatrixOffset.setTranslation(sideLeftShoulder);
	shoulder2MatrixOffset.setTranslation(sideRightShoulder);

	FVector weaponSocketFront(armScaleCM * 0.8f, 0.0f, armScaleCM); //same height as arm, forward is X
	weaponMatrixOffset.setTranslation(weaponSocketFront);
}


void BoneController::setupAnimation(){
    KeyFrameAnimation legAnimationKeys = KeyFrameAnimation(false); //instant flip animation
	legAnimationKeys.addFrame(
		FVector(0, 0, -legScaleCM),
		0.0f, //time to prev frame
		false,
		legScaleCM
	);
	legAnimationKeys.addFrame(
		FVector(25, 0, -legScaleCM * 0.9),
		0.5f,
		false,
		legScaleCM
	);
	legAnimationKeys.addFrame(
		FVector(50, 0, -legScaleCM),
		0.5f,
		true, //GROUNDED
		legScaleCM
	);

	KeyFrameAnimation legKeysCopy = legAnimationKeys;

	legDoubleKeys_1.setAnimationA(MoveTemp(legAnimationKeys));
	legDoubleKeys_2.setAnimationA(MoveTemp(legKeysCopy));

	FVector hipTargetFromFoot = FVector(0, 0, legScaleCM);
	legDoubleKeys_1.setAnimationBAdjustPermanentTarget(hipTargetFromFoot);
	legDoubleKeys_2.setAnimationBAdjustPermanentTarget(hipTargetFromFoot);


	//set aniatmion running true or false, 
	legDoubleKeys_1.setRunning(isRunning); //member var from this
	legDoubleKeys_2.setRunning(isRunning);




	//setup climbing
	KeyFrameAnimation climbAnimationKeys = KeyFrameAnimation(false); //instant flip animation
	climbAnimationKeys.addFrame(
		FVector(0, 0, -armScaleCM),
		0.0f, //time to prev frame
		false,
		armScaleCM
	);
	climbAnimationKeys.addFrame(
		FVector(20, 0, -armScaleCM),
		0.5f, //time to prev frame
		false,
		armScaleCM
	);
	climbAnimationKeys.addFrame(
		FVector(50, 0, armScaleCM), //nach oben strecken
		0.5f, //time to prev frame
		true, //GROUNDED
		armScaleCM
	);
	armClimbKeys_1.setAnimationA(MoveTemp(climbAnimationKeys));
	FVector hipTargetFromHand = FVector(0, 0, armScaleCM); //von hand yu hip, was ist der default target wert
	armClimbKeys_1.setAnimationBAdjustPermanentTarget(hipTargetFromHand);

	armClimbKeys_1.setRunning(false);
}



void BoneController::SetLocation(FVector &pos){
    //setup
    ownLocation.setTranslation(pos);

    ownLocationFoot1.setTranslation(pos);
    ownLocationFoot2.setTranslation(pos);

	ownLocationHand1.setTranslation(pos);
    ownLocationHand2.setTranslation(pos);

}

FVector BoneController::GetLocation(){
	return currentTransform().getTranslation();
}

/// @brief is not tested!
/// @param TargetLocation 
void BoneController::LookAt(FVector TargetLocation) 
{
    // Calculate the rotation needed to look at the target location
	MMatrix transform = currentTransform();
	FVector location = transform.getTranslation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(location, TargetLocation);

	//FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	float zDegree = LookAtRotation.Yaw;

	ownOrientation.yawRad(MMatrix::degToRadian(zDegree));
}

void BoneController::yawRotate(float degree){
	float rad = MMatrix::degToRadian(degree);
	ownOrientation.yawRad(rad);
}

/// @brief method to attach a carried item to the hands of the actor
/// @param carriedItem 
void BoneController::attachCarriedItem(AcarriedItem *carriedItem){
	if(carriedItem != nullptr){
		attachedCarriedItem = carriedItem;
		attachedCarriedItem->SetActorEnableCollision(false);
	}
}




MMatrix BoneController::currentFootTransform(MMatrix &foottranslationToRotate){
	//M = T * R <-- lese richtung --
	MMatrix rotationTransform = foottranslationToRotate * ownOrientation; //rotiert fuss das es übereinstimmt mit actor
	return rotationTransform;
}



/// @brief returns the current translation and rotation in world in correct order as expected
/// HIP PIVOT TRANSFORM ONLY
/// @return current Transform Matrix by value
MMatrix BoneController::currentTransform(){ //might be renamed to hip pivot 
	MMatrix rotationTransform = ownLocation * ownOrientation;
	return rotationTransform;
}


MMatrix BoneController::currentTransform(MMatrix &offset){
	//M = T * R * Toffset <-- lese richtung --
    MMatrix RT = ownOrientation * offset; //erst schulter offset z.b.
    MMatrix rotationTransform = ownLocation * RT; //rotiert fuss das es übereinstimmt mit actor
    return rotationTransform;
}


MMatrix BoneController::currentTransform(int leg){
    if(leg == FOOT_1){
		return currentTransform(hip1MatrixOffset);
    }
    if(leg == FOOT_2){
        return currentTransform(hip2MatrixOffset);
    }
	if(leg == SHOULDER_1){
		return currentTransform(shoulder1MatrixOffset);
    }
    if(leg == SHOULDER_2){
        return currentTransform(shoulder2MatrixOffset);
    }

    return currentTransform();
}

MMatrix BoneController::offsetMatrixByLimb(int limb){
	if(limb == FOOT_1){
		return hip1MatrixOffset;
    }
    if(limb == FOOT_2){
        return hip2MatrixOffset;
    }
	if(limb == SHOULDER_1){
		return shoulder1MatrixOffset;
    }
    if(limb == SHOULDER_2){
        return shoulder2MatrixOffset;
    }
	return MMatrix();
}

/// @brief creates the inverse matrix to remove from a position 
/// to create the hip position
/// @param limb 
/// @return inverse matrix to remove the rotated limb offset
MMatrix BoneController::offsetInverseMatrixByLimb(int limb){
	MMatrix offsetLimb = offsetMatrixByLimb(limb); // T
	MMatrix offset = ownOrientation * offsetLimb; // M = R * T <-- lese richtung (erst offset) --
	
	return offset.createInverse();
}



/// @brief converts a position to relative to the hip / actor center
/// @param position position to convert, will be adjusted by reference
void BoneController::transformFromWorldToLocalCoordinates(FVector &position, int limb){

	//M = C * B * A
	/*
	MMatrix A = offsetMatrixByLimb(limb).createInverse();
	MMatrix B = ownOrientation.createInverse();
	MMatrix C = ownLocation.createInverse();

	MMatrix inverse = C * B;
	inverse *= A;
	*/
	

	// testing (scheint zu funktionieren)
	MMatrix inverse = currentTransform(limb).jordanInverse();

	position = inverse * position;
}






/// @brief update hip location relative to some leg / bone starting joint 
/// (The inverse will be taken and position of hip / root adjusted accordingly)
/// (shoulder / hip offset)
/// @param updatetHipJointMat updated matrix (relative root transform, for example shoulder) 
/// @param limbindex limb index of the limb 
void BoneController::updateHipLocation(MMatrix &updatetHipJointMat, int limbindex){
    
	//mit inverse die matrix zurück rechnen und root setzen.
	
	//offset invertieren
	MMatrix offset = offsetInverseMatrixByLimb(limbindex);
	updatetHipJointMat *= offset; //M = T * back  <-- lese richtung --
	ownLocation = updatetHipJointMat;
	
}





void BoneController::SetControllerState(BoneControllerStates state){
    currentMotionState = state;
}


//will be designed for arms
void BoneController::Tick(float DeltaTime, UWorld *worldIn, FVector overrideLocation){
	ownLocation.setTranslation(overrideLocation);
	Tick(DeltaTime, worldIn);
}

/// @brief must be ticked from outside skelletal owner
///
/// @param DeltaTime 
/// @param actorLocation ---> new location for this bone controller, COPIED MATRIX!
/// @param world ---> world is passed AND SAVED!, caution 
void BoneController::Tick(float DeltaTime, UWorld *worldIn){
    world = worldIn;

	

    if(currentMotionState == BoneControllerStates::none){
        TickBuildNone(DeltaTime);
    }
    if(currentMotionState == BoneControllerStates::locomotion){
        TickLocomotion(DeltaTime);
		TickArms(DeltaTime);
	}
	
	if(currentMotionState == BoneControllerStates::locomotionClimb){
		TickLocomotionClimb(DeltaTime);
		TickLegsNone(DeltaTime);
	}
	if(currentMotionState == BoneControllerStates::locomotionClimbAdjust){
		TickLocomotionClimbAdjust(DeltaTime);
		TickArms(DeltaTime);
	}
	


	if(currentMotionState == BoneControllerStates::locomotionClimbAll){
		TickLocomotionClimbAll(DeltaTime);
	}
}



/// @brief Tick Locomotion Based on State 
/// @param DeltaTime 
void BoneController::TickLocomotion(float DeltaTime){

	
	if (leg1isPlaying)
	{
		buildRawAndKeepEndInPlace(leg2, ownLocationFoot2, DeltaTime, leg2Color, FOOT_2);
		playForwardAndBackwardKinematicAnim(leg1, legDoubleKeys_1, ownLocationFoot1, DeltaTime, leg1Color, FOOT_1);

		//switch to next leg but update relative position
		if(legDoubleKeys_1.animationCycleWasComplete()){
			leg1isPlaying = false;

			//die hip wurde mit dem anderen bein bewegt /(leg 1, also muss leg2 geupdated werden)
			FVector footPos = ownLocationFoot2.getTranslation();
			DebugHelper::showLineBetween(world, footPos, currentTransform(FOOT_2).getTranslation(), FColor::Orange, 2.0f);


			transformFromWorldToLocalCoordinates(footPos, FOOT_2);
			legDoubleKeys_2.overrideCurrentStartingFrame(footPos); //update starting pos of the foot because the actor moved

			
		}
	}else{
		
		buildRawAndKeepEndInPlace(leg1, ownLocationFoot1, DeltaTime, leg1Color, FOOT_1);
		playForwardAndBackwardKinematicAnim(leg2, legDoubleKeys_2, ownLocationFoot2, DeltaTime, leg2Color, FOOT_2);

		if(legDoubleKeys_2.animationCycleWasComplete()){//left leg done
			leg1isPlaying = true;

			//die hip wurde mit dem anderen bein bewegt /(leg 2, also muss leg1 geupdated werden)
			FVector footPos = ownLocationFoot1.getTranslation();
			DebugHelper::showLineBetween(world, footPos, currentTransform(FOOT_1).getTranslation(), FColor::Orange, 2.0f);
			
            transformFromWorldToLocalCoordinates(footPos, FOOT_1);
			legDoubleKeys_1.overrideCurrentStartingFrame(footPos); //update starting pos of the foot because the actor moved

			
		}
	}

	
}


/// @brief locomotion for climbing with arms -> must be set to locomotion climb if
/// the projected leg locomotion is too far up.
/// once the climbing is complete its switched to locomotion climb adjust,
/// leg 1 will complete the climb
/// @param DeltaTime 
void BoneController::TickLocomotionClimb(float DeltaTime){
	//es braucht eine temporäre double keyframe kette für beide arme
	//für temporäres hochziehen was einmal ausgeführt wird

	

	//switch to next leg but update relative position
	
	if(armClimbKeys_1.animationCycleWasComplete()){

		//switch to climb adjust (bein aufsetzen)
		currentMotionState = BoneControllerStates::locomotionClimbAdjust;

		FVector footPos = ownLocationFoot1.getTranslation();
        transformFromWorldToLocalCoordinates(footPos, FOOT_1);

		//hand effector als target (funktioniert so)
		FVector frame = ownLocationHand1.getTranslation();
		transformFromWorldToLocalCoordinates(frame, FOOT_1);
		legDoubleKeys_1.skipAnimationOnce(footPos, frame);

		/**
		 * DEBUG WISE ADDED 
		 * Works as expected if single bone.
		 
		footPos = ownLocationFoot1.getTranslation();
		frame = ownLocationHand1.getTranslation();
		legDoubleKeys_1.skipAnimationOnceWorld(footPos, frame);*/
		/**
		 * Reset der rotation hier notwendig weil nach
		 * einem backward build, einen raw build durchzuführen,
		 * alle rotationen falsch sind (noch im umgedrehten zustand).
		 * Wenn der arm in der ausgangs lage nun ist,
		 * ist das resetten korrekt.
		 * Wenn nicht, muss zum ausgangs punkt (initial target von start 
		 * effector(schulter) aus gesehen) hinrotiert werden.
		 */
		arm1.resetAllRotations(); //TESTING DEBUGGING ROTATION BUG! 

		return;
	}

	//sollte erstmal auch einfach nur ein arm sein

	//problem: teilweise wird ein arm zu lange abgespielt was auch echt komisch ist
	//nach dem adjust glitched der nochmal
	/** old working
	playForwardAndBackwardKinematicAnim(
		arm1, 
		armClimbKeys_1, 
		ownLocationHand1, 
		DeltaTime, 
		arm1Color, 
		SHOULDER_1
	);*/


	/**
	 * Debug testing, geht soweit 
	 * */
	std::vector<int> limbs;
	std::vector<DoubleKeyFrameAnimation *> animations;
	limbs.push_back(SHOULDER_1);
	animations.push_back(&armClimbKeys_1);

	playForwardAndBackwardKinematicAnimSynchronized(
		DeltaTime,
		limbs,
		animations
	);





	//will skip arms which are invloved in climbing
	TickArms(DeltaTime);
}


/// @brief plays the locomotion climb adjusting for the 
/// first leg until the adjustment is complete. After adjusting (animation cycle is complete)
/// its switched back to default locomotion (walking)
/// @param DeltaTime 
void BoneController::TickLocomotionClimbAdjust(float DeltaTime){

	
	
	//switch to next leg but update relative position
	if(legDoubleKeys_1.animationCycleWasComplete()){
		leg1isPlaying = false;

		MMatrix currentTransformLeg2 = currentTransform(FOOT_2);

		//die hip wurde mit dem anderen bein bewegt /(leg 1, also muss leg2 geupdated werden)
		FVector footPos = ownLocationFoot2.getTranslation();
		
		transformFromWorldToLocalCoordinates(footPos, FOOT_2);
		legDoubleKeys_2.overrideCurrentStartingFrame(footPos); //update starting pos of the foot because the actor moved
		legDoubleKeys_2.forceProjectNextFrame(world, currentTransformLeg2);

		currentMotionState = BoneControllerStates::locomotion;
		return;
	}

	//playForwardAndBackwardKinematicAnim(leg1, legDoubleKeys_1, ownLocationFoot1, DeltaTime, leg1Color, FOOT_1);
	
	//debug testing
	std::vector<int> limbs;
	std::vector<DoubleKeyFrameAnimation *> animations;
	limbs.push_back(FOOT_1);
	animations.push_back(&legDoubleKeys_1);

	playForwardAndBackwardKinematicAnimSynchronized(
		DeltaTime,
		limbs,
		animations
	);
	
	
	
	
	
	
	
	TickLimbNone(FOOT_2, DeltaTime);
}



















/// @brief will tick the arms and legs no motion
/// @param DeltaTime delta time from last tick
void BoneController::TickBuildNone(float DeltaTime){
	
    //hier müssen alle bones einfach gebaut werden
	TickArms(DeltaTime);
	TickLegsNone(DeltaTime);
}


/// @brief will tick the arm movement and exclude arms
/// if they are climbing
/// call this method for any arm related tick
/// @param DeltaTime 
void BoneController::TickArms(float DeltaTime){

	if(
		currentMotionState != BoneControllerStates::locomotionClimb &&
		currentMotionState != BoneControllerStates::locomotionClimbAll
	){
		TickLimbNone(SHOULDER_1, DeltaTime); //unklar ob andere hand auch waffe halten soll
	}

	
	
	
	//TickLimbNone(SHOULDER_2, DeltaTime); //holds weapon now. 

	/**
	 * TESTING MOVE ARM TO TARGET WEAPON
	 * ---- EXPIREMENTAL ----
	 * 
	 * ----> arm 2 always holds the gun now
	 */
	if(attachedCarriedItem != nullptr){

		//move weapon by this actor for now
		MMatrix weaponWorldMatrix = currentTransform(weaponMatrixOffset);
		FVector locationWeapon = weaponWorldMatrix.getTranslation();
		attachedCarriedItem->SetActorLocation(locationWeapon); //update location
		FRotator rotator = ownOrientation.extractRotator();
		attachedCarriedItem->SetActorRotation(rotator);


		//attach hand
		FVector targetWorld = attachedCarriedItem->rightHandLocation(); // arm 2 (?)
		//DebugHelper::showScreenMessage("target: ", targetWorld, FColor::Emerald);
		FVector targetLocal = targetWorld;
		transformFromWorldToLocalCoordinates(targetLocal, SHOULDER_2);

		FColor color = limbColor(SHOULDER_2);
		FVector weight(-1, 0, 0);
		MMatrix translationActor = currentTransform(SHOULDER_2);

		arm2.rotateEndToTargetAndBuild(
			GetWorld(),
			targetLocal,
			weight,
			translationActor, // hip start with orient
			ownLocationHand2, //ownLocationFoot,  // foot apply positions
			color, 
			DeltaTime * 2.0f
		);
	}
}




/// @brief build both legs, no motion
/// @param DeltaTime 
void BoneController::TickLegsNone(float DeltaTime){
	TickLimbNone(FOOT_1, DeltaTime);
	TickLimbNone(FOOT_2, DeltaTime);
}

void BoneController::TickLimbNone(int limbIndex, float DeltaTime){
	
	TwoBone *bone = findBone(limbIndex);
	MMatrix *endEffector = findEndEffector(limbIndex);

	if(bone != nullptr && endEffector != nullptr){

		MMatrix transform = currentTransform(limbIndex);

		FVector weight(1, 0, 0);
		FVector target = endEffector->getTranslation();
		transformFromWorldToLocalCoordinates(target, limbIndex);

		bone->build(
			GetWorld(), 
			transform, 
			*endEffector, //by reference updatet
			limbColor(limbIndex), 
			DeltaTime * 2.0f
		);		
		
	}
}

/**
 * 
 * ----- ARM FORWARD IK SECTION ------
 * 
 */


/**
 * soll den knochen bauen anhand animation und einen offset vorgeben
 * 
 * ----- todo: refacture / remove! --> index sollte übergeben des bones
 */

void BoneController::playForwardKinematicAnim(
	TwoBone &bone, 
	KeyFrameAnimation &frames, 
	float DeltaTime,
	int limbIndex
){

	MMatrix initalTransform = currentTransform(limbIndex);
	FColor color = limbColor(limbIndex);

	//testing
	FVector weight(-1, 1, 0); //testing needed for arm rotation
	FVector nextPos = frames.interpolate(DeltaTime); //new function
	
	//dummy adjust, not foot or end, nothing for now. Could be hand.
	MMatrix dummy;

	MMatrix *endEffector = findEndEffector(limbIndex);
	MMatrix &ref = dummy; //is dummy if none found
	if(endEffector != nullptr){
		ref = *endEffector;
	}

	//todo hier: end effector einfügen!

	bone.rotateEndToTargetAndBuild(
		GetWorld(),
		nextPos,
		weight,
		initalTransform, //offset and rotation
		ref, //end effector to update
		color,
		DeltaTime * 2.0f
	);
}












/**
 * 
 * 
 * ----- HIP SECTION / DOUBLE KEY FRAME ANIMATION ------
 * 
 * 
 */

/// @brief renders the leg raw if the hip is moved!
/// @param boneIk bone to render which foot stays in place
/// @param legEndTransform leg position to hold and not move from
/// @param deltaTime delta time
/// @param color color to render
void BoneController::buildRawAndKeepEndInPlace(
    TwoBone &boneIk, 
    MMatrix &legEndTransform, 
    float deltaTime, 
    FColor color,
    int leg
){
    
    

    /*
	build leg raw

	ziel hier: 
	von hier das bein so rendern das bein am originalen punkt bleibt
	das foot transform muss also zu einem target umgewandelt werden
	und dann dort hin bewegt. Egal ob was neu berechnet wird oder nicht
	*/

	FVector weight(1, 0, 0); // gewicht zieht nach vorne
	
	FVector pos = legEndTransform.getTranslation();
    MMatrix currenttransform = currentTransform(leg);
	currenttransform.transformFromWorldToLocalCoordinates(pos); //relativ zur hüfte neu berechnen, auchwenn hüfte sich bewegt!

	boneIk.rotateEndToTargetAndBuild(
		GetWorld(),
		pos,
		weight,
		currenttransform, // MMatrix &offsetAndRotation,
		legEndTransform,
		color,//FColor::Red,
		deltaTime * 2.0f // displayTime
	);

}





/**
 * 
 * 
 * FOOT ANIMATION! 
 * 
 * 
 */


void BoneController::playForwardAndBackwardKinematicAnim(
	TwoBone &bone, 
	DoubleKeyFrameAnimation &frames, 
	MMatrix &footMatrix, //MMatrix foot transform 
	float DeltaTime,
	FColor color,
    int leg //bein 1 oder 2 fürs erste, könnte auch liste werden
){
	
	bool moveLeg = frames.isAnimationA();

	if(moveLeg){
        // ---- FORWARD BUILD ----
		playForwardKinematicAnim(bone, frames, footMatrix, DeltaTime, color, leg);	
	}else{
		// ---- HIP ADJUST BUILD ----
		playBackwardKinematicAnim(bone, frames, footMatrix, DeltaTime, color, leg);
	}
}




/// @brief 
/// @param bone 
/// @param frames 
/// @param footMatrix 
/// @param DeltaTime 
/// @param color 
/// @param leg 
void BoneController::playForwardKinematicAnim(
	TwoBone &bone, 
	DoubleKeyFrameAnimation &frames, 
	MMatrix &footMatrix,
	float DeltaTime,
	FColor color,
    int leg
){

	// add velocity and project frames 
    MMatrix current = currentTransform(leg); //based on leg transform
	
	float velocityT = addVelocityBasedOnState(); 
    FVector lookDir = currentTransform().lookDirXForward();
	lookDir.Z = 0.0f; //xy pane only of interest
	bool switchToArmLocomotion = false;
	//nicht projezieren wenn grade adjust ist zum klettern (bein hoch ziehen)
	if(currentMotionState != BoneControllerStates::locomotionClimbAdjust){ 
		frames.projectNextFrameIfNeeded(
			GetWorld(), 
			current,
			velocityT,
			lookDir,
			switchToArmLocomotion, //boolean by reference, switch needed or not
			legScaleCM, //max height 
			currentMotionState
		);
	}
	
	//override motion state if needed
	if(switchToArmLocomotion && currentMotionState == BoneControllerStates::locomotion){
		currentMotionState = BoneControllerStates::locomotionClimb;
		
		//debug override to test new animation overlapped method
		currentMotionState = BoneControllerStates::locomotionClimbAll; //debug test
	}

	FVector thisAdd = lookDir * velocityT * DeltaTime;
	ownLocation += thisAdd; //running velocity, sollte auch hier so passen im Bone Controller




	/**
	 * HIP PROJECTION ADJUST DO NOT REMOVE! 
	 * Adds the needed hip offset over time
	 * for the hip when the point gets projected to the floor
	*/
	MMatrix translationActorTmp = currentTransform(); 
	FVector hipoffsetAdd = frames.getProjectionHipOffsetTimed(DeltaTime);
	ownLocation += hipoffsetAdd; 

	DebugHelper::showLineBetween(
			GetWorld(), 
			translationActorTmp * hipoffsetAdd, 
			translationActorTmp.getTranslation(), 
			FColor::Red, 
			2.0f
	);


	//do movement
	//nochmal neu rechnen nach offset
    MMatrix translationActor = currentTransform(leg);	
	FVector weight(1, 0, 0);


	//end effector update - old
	FVector nextPos;

	

	// WORKSSS!!!!
	FVector EndEffectorWorld = footMatrix.getTranslation();
	nextPos = frames.interpolateWorld(
		DeltaTime,
		EndEffectorWorld,
		translationActor
	);
	


	bone.rotateEndToTargetAndBuild(
		GetWorld(),
		nextPos,
		weight,
		translationActor, // hip start with orient
		footMatrix, //ownLocationFoot,  // foot apply positions
		color, 
		DeltaTime * 2.0f
	);
}


/// @brief plays the animation and adjusts the hip of the actor based on the movement made
/// the end effector will be kept in place
/// @param bone 
/// @param frames 
/// @param footMatrix 
/// @param DeltaTime 
/// @param color 
/// @param leg 
void BoneController::playBackwardKinematicAnim(
	TwoBone &bone, 
	DoubleKeyFrameAnimation &frames, 
	MMatrix &footMatrix, //MMatrix foot transform 
	float DeltaTime,
	FColor color,
    int leg
){

	FVector weight(1, 0, 0);

	//offset for foot during hip adjust movement
	FVector hipoffsetAdd = frames.getProjectionHipOffsetTimed(DeltaTime);
	footMatrix += hipoffsetAdd;
	

	//build
	//FVector xt = frames.interpolate(DeltaTime);

	
	//new live update position at all times
	FVector StartEffectorWorld = currentTransform(leg).getTranslation();
	MMatrix inverse = currentFootTransform(footMatrix).jordanInverse();
	FVector startEffectorLocal = inverse * StartEffectorWorld;
	FVector xt = frames.interpolate(DeltaTime, startEffectorLocal); //interpolated with live location update
	
	//draw
	FVector copy = startEffectorLocal;
	FVector start = footMatrix.getTranslation();
	DebugHelper::showLineBetween(GetWorld(), start + copy, start, FColor::Cyan, 5.0f);

	


	MMatrix current_RotatedFoot = currentFootTransform(footMatrix);

    //hip matrix aber da wo der fuss attached ist am oberschenkel
    MMatrix ownLocationRelativeToHip = currentTransform(leg);
    
    //MMatrix current_translationActorFoot = currentFootTransform(footMatrix);
	bone.rotateStartToTargetAndBuild(
		GetWorld(),
		xt,
		weight,
		current_RotatedFoot, // foot start
		ownLocationRelativeToHip, // hip apply HIER MUSS DIE LIMB PASSENDE HIP MATRIX REIN!!!
		color, //FColor::Blue,
		DeltaTime * 2.0f
	);

    //inverse berechnen sodass man den offset zurück wandert und hip beinflusst
	//wichtig!
	updateHipLocation(ownLocationRelativeToHip, leg);

}




UWorld *BoneController::GetWorld(){
    return world;
}



float BoneController::addVelocityBasedOnState(){
	if(
		currentMotionState == BoneControllerStates::locomotion &&
		isRunning
	){
		return 200.0f;
	}
	return 0.0f;
}















/**
 * 
 * 
 * ---  new testing region for synchronus movement of bones with BK ---
 * 
 */


void BoneController::TickLocomotionClimbAll(float DeltaTime){



	
	//switch to next leg but update relative position
	
	//hand reached target, end effector bleibt gelockt für das bein
	if(armClimbKeys_1.isAnimationB() && !climb_setHandTarget){ 

		climb_setHandTarget = true;
		
		//neu:
		FVector footPos = ownLocationFoot1.getTranslation();
		FVector frame = ownLocationHand1.getTranslation();
		MMatrix startMat = currentTransform(FOOT_1);
		legDoubleKeys_1.skipAnimationOnceWorld(startMat, footPos, frame);

		/*
		FVector footPos = ownLocationFoot1.getTranslation();
        transformFromWorldToLocalCoordinates(footPos, FOOT_1);

		//hand effector als target (funktioniert so bei der anderen methode)
		FVector frame = ownLocationHand1.getTranslation();
		transformFromWorldToLocalCoordinates(frame, FOOT_1);
		legDoubleKeys_1.skipAnimationOnce(footPos, frame);
		*/



		DebugHelper::showLineBetween(
			GetWorld(),
			footPos,
			frame,
			FColor::Yellow,
			10.0f
		);
		
	}
	
	//END OF ANIMATION ALL
	//switch to next leg but update relative position
	if(legDoubleKeys_1.isAnimationB() && climb_hand1cycleComplete){
		
		currentMotionState = BoneControllerStates::locomotion;
		climb_setHandTarget = false;
		climb_hand1cycleComplete = false;
		return;
	}
	
	if(armClimbKeys_1.animationCycleWasComplete()){
		if(climb_hand1cycleComplete == false){
			arm1.resetAllRotations();
		}

		climb_hand1cycleComplete = true;

	}


	//build bones
	std::vector<int> limbs;
	std::vector<DoubleKeyFrameAnimation *> animations;


	//nur anticken wenn man sich hochzieht, und noch nicht fertig war
	if(!climb_hand1cycleComplete){
		limbs.push_back(SHOULDER_1);
		animations.push_back(&armClimbKeys_1);
	}else{
		TickLimbNone(SHOULDER_1, DeltaTime);
	}

	if(climb_setHandTarget){ //debug: force linear, geht nicht
		limbs.push_back(FOOT_1);
		animations.push_back(&legDoubleKeys_1);
	}
	

	playForwardAndBackwardKinematicAnimSynchronized(
		DeltaTime,
		limbs,
		animations
	);




	// will skip arms which are invloved in climbing
	TickArms(DeltaTime);
	TickLimbNone(FOOT_2, DeltaTime);

	
}




/**
 * tested for single arm climb movement
 */
void BoneController::playForwardAndBackwardKinematicAnimSynchronized(
	float DeltaTime,
	std::vector<int> limbs,
	std::vector<DoubleKeyFrameAnimation*> animations
){
	bool backwardKinematicIsBlocked = false;
	for (int i = 0; i < limbs.size(); i++){
		if(i < animations.size()){
			int limbIndex = limbs[i];
			DoubleKeyFrameAnimation *frames = animations[i];
			TwoBone *bone = findBone(limbIndex);
			MMatrix *effectorMat = findEndEffector(limbIndex);
			FColor color = limbColor(limbIndex);
			if (
				frames != nullptr &&
				bone != nullptr &&
				effectorMat != nullptr
			){
				bool moveLeg = frames->isAnimationA();
				if(moveLeg){
					// ---- FORWARD BUILD ----
					playForwardKinematicAnim(*bone, *frames, *effectorMat, DeltaTime, color, limbIndex);	
				}
				if(!moveLeg && !backwardKinematicIsBlocked){
					// ---- HIP ADJUST BUILD ----
					playBackwardKinematicAnim(*bone, *frames, *effectorMat, DeltaTime, color, limbIndex);
					backwardKinematicIsBlocked = true;

					FString text = FString::Printf(TEXT("blocked backward adjust by  %d"), limbIndex);
					DebugHelper::logMessage(text);
				}
			}
		}
	}
	
}