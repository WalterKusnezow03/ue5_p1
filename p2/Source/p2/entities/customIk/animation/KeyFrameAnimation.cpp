// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/TargetInterpolator.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"

KeyFrameAnimation::KeyFrameAnimation()
{
    frameIndex = 0;
    nextFrameIndex = 1;
    totalLengthSave = 0.0f;
    loop = true;
}

KeyFrameAnimation::KeyFrameAnimation(bool loopIn){
    frameIndex = 0;
    nextFrameIndex = 1;
    totalLengthSave = 0.0f;
    loop = loopIn;
}


KeyFrameAnimation::~KeyFrameAnimation()
{
}





/// @brief add a frame to the animation at a position and distance to previous frame
/// @param position position for frame
/// @param timeFromLastFrame time to keep from last frame in seconds
void KeyFrameAnimation::addFrame(FVector position, float timeFromLastFrame){
    addFrame(position, timeFromLastFrame, false);
}

void KeyFrameAnimation::addFrame(FVector position, float timeFromLastFrame, bool mustBeGrounded){
    totalLengthSave += timeFromLastFrame;
    frames.push_back(KeyFrame(position, timeFromLastFrame, mustBeGrounded));
}

void KeyFrameAnimation::addFrame(
    FVector position,
    float timeFromLastFrame,
    bool mustBeGrounded,
    float clampDistance
){
    FVector zeroVec(0, 0, 0);
    if(FVector::Dist(zeroVec, position) > clampDistance){
        position = position.GetSafeNormal() * clampDistance;
    }

    totalLengthSave += timeFromLastFrame;
    frames.push_back(KeyFrame(position, timeFromLastFrame, mustBeGrounded));
}





/// @brief total length time of the animation
/// @return total length time of the animation
float KeyFrameAnimation::totalLength(){
    return totalLengthSave;
}

/// @brief returns whether the end frame was reached, only is flagged if the animation
/// was not initialized with loop, NOT LOOP!
/// @return reached last frame flag
bool KeyFrameAnimation::reachedLastFrameOfAnimation(){
    if(reachedEndFrameFlag){
        reachedEndFrameFlag = false;
        return true;
    }
    return false;
}



/// @brief returns if enough frames are available for an animation (at least 2)
/// @return has enough or not
bool KeyFrameAnimation::hasAnyFrames(){
    return frames.size() >= 2; //must be at least 2!
}




FVector KeyFrameAnimation::interpolate(float DeltaTime){

    if(hasAnyFrames()){
    
        if (interpolator.hasTargetSetup() == false){
            updateFrameInterpolator();
        }

        FVector interpolated = interpolator.interpolate(DeltaTime);
        

        if(interpolator.hasReachedTarget()){    
            updateFrameIndex();
            updateFrameInterpolator();
        }

        return interpolated;
    }
    DebugHelper::showScreenMessage("not enough frames!");
    return FVector(0, 0, 0);
}   

/// @brief updates the frame index and end frame flag if reached
void KeyFrameAnimation::updateFrameIndex(){
    frameIndex = (frameIndex + 1) % frames.size();
    nextFrameIndex = (frameIndex + 1) % frames.size();

    bool isEnd = (nextFrameIndex == 0) || (frameIndex == frames.size() - 1);
    if (isEnd && !loop)
    {
        frameIndex = 0;
        nextFrameIndex = 1;
        //DebugHelper::showScreenMessage("reached end frame!", FColor::Orange);
        reachedEndFrameFlag = true;
    }

}

/// @brief call update AFTER increase / index update
void KeyFrameAnimation::updateFrameInterpolator(){

    KeyFrame &currentFrame = frames.at(frameIndex);
    KeyFrame &nextFrame = frames.at(nextFrameIndex);
    interpolator.setTarget(
        currentFrame.readposition(),
        nextFrame.readposition(),
        nextFrame.timeToFrame()
    );

    //reset frame projected status
    frameIsProjected = false;
}


bool KeyFrameAnimation::nextFrameMustBeGrounded(){
    if(hasAnyFrames()){
        KeyFrame &nextFrame = frames.at(nextFrameIndex);
        return nextFrame.mustBeGrounded();
    }
    return false;
}

FVector KeyFrameAnimation::readNextFrame(){
    if(hasAnyFrames()){
        KeyFrame &nextFrame = frames.at(nextFrameIndex);
        return nextFrame.readposition();
    }
    return FVector(0, 0, 0);
}



/// @brief override the next frame / target value, animation is not overriden, individual to current next
/// frame!
/// @param framePos 
void KeyFrameAnimation::overrideNextFrame(FVector &framePos){
    interpolator.overrideTarget(framePos);
    frameIsProjected = true;
}



bool KeyFrameAnimation::nextFrameIsProjected(){
    return frameIsProjected;
}




/// @brief pushes a position to the front of the animation temporarly just once and resets the index
/// if the position is far engough from the current starting frame 
/// designed for foot to correct its position because another foot moved and changed the relative position!
/// @param somePosition 
void KeyFrameAnimation::tryPushFront(FVector &somePosition, float time){

    //frameIndex = frames.size() -1;
    //nextFrameIndex = 0;
    interpolator.insertAtFront(somePosition);
    
}




bool KeyFrameAnimation::projectNextFrameToGroundIfNeeded(UWorld *world, MMatrix &actorMatrix, FVector &offsetMade){
    if(world == nullptr){
        return false;
    }
    if(!nextFrameIsProjected() && nextFrameMustBeGrounded()){
        FVector frameToProject = readNextFrame();
        projectToGround(world, actorMatrix, frameToProject, offsetMade);
        overrideNextFrame(frameToProject);
        return true;
    }
    return false;
}







/**
 * 
 * 
 * PROJEKTION SOLLTE HIER MÖGLICH SEIN
 * 
 * 
 */

/// @brief projects a frame to the ground and writes the porjection into the frame
/// @param frameToProject frame to project to floor
/// @param offsetMade offset made from frame to project to hitpoint direction
void KeyFrameAnimation::projectToGround(
    UWorld *world, MMatrix &actorTransform, FVector &frameToProject, FVector &offsetMade
){
    if(world == nullptr){
        return;
    }
    MMatrix transform = actorTransform;
    FVector frameInWorld = transform * frameToProject;

    //EXTRA OFFSET NEEDED HERE, sont terrain nicht berührt, könnte durchlaufen!
	frameInWorld += FVector(0, 0, 200); //TEST RAYCAST START NACH OBEN!

	//project frame to floor
	FVector downVec(0, 0, -1);
	FVector hitpoint;

	bool wasHit = performRaycast(world, frameInWorld, downVec, hitpoint);
	if(wasHit){
		
		actorTransform.transformFromWorldToLocalCoordinates(hitpoint);
		offsetMade = hitpoint - frameToProject;
		frameToProject = hitpoint;
	}
}




/// @brief performs a raycast from a start, into a direction, with a max length in this method
/// @param Start start position
/// @param dir direction of interest
/// @param outputHit output hit if any hit happened
/// @return bool anything was hit
bool KeyFrameAnimation::performRaycast(UWorld *world, FVector &Start, FVector &dir, FVector &outputHit) {

    if(world == nullptr){
        return false;
    }

    float scaleOfVector = 1000; //some random value for now
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
	bool bHit = world->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	



	//anything was hit
	if (bHit){
		outputHit = HitResult.ImpactPoint; //write impactpoint to output
		return true;
	}

	return false;
}







/**
 * 
 * 
 * NEW! TESTING FUTURE TIME FRAMES
 * 
 * ---- new section for future projection: velocity and direction? ----
 * 
 * 
 */

float KeyFrameAnimation::readNextTimeToFrame(){
    if(hasAnyFrames()){
        KeyFrame &nextFrame = frames.at(nextFrameIndex);
        return nextFrame.timeToFrame();
    }
    return 0.0f;
}

bool KeyFrameAnimation::projectNextFrameToGroundIfNeeded(
    UWorld *world, 
    MMatrix &actorMatrix, 
    FVector &offsetMade,
    float velocity,
    FVector &lookdir
){
    if(world == nullptr){
        return false;
    }
    if(!nextFrameIsProjected() && nextFrameMustBeGrounded()){
        FVector frameToProject = readNextFrame();

        //hier dann auch time to frame holen für future projektion des frames
        float nextTimeToFrame = readNextTimeToFrame(); 

        projectToGround(
            world, 
            actorMatrix, 
            frameToProject,
            offsetMade, 
            nextTimeToFrame,
            velocity,
            lookdir
        );
        
        overrideNextFrame(frameToProject);
        return true;
    }
    return false;
}

/// @brief projects a frame to the ground and writes the porjection into the frame
/// @param frameToProject frame to project to floor, will be modified!
/// @param offsetMade offset made from frame to project to hitpoint direction
void KeyFrameAnimation::projectToGround(
    UWorld *world, 
    MMatrix &actorTransform, 
    FVector &frameToProject, 
    FVector &offsetMade,
    float timeToFrame,
    float velocity, 
    FVector &lookdirection
){
    if(world == nullptr){
        return;
    }
    MMatrix transform = actorTransform;
    FVector frameInWorld = transform * frameToProject;

    //hier einfach drauf rechnen? (velocity und lookdir)
    //muss in abhängigkeit mit time to frame sein.

    //x(t) = x0 + v0t + 1/2 at^2
    //simplified to
    //x(t) = x0 + v0 t
    //xthis(t) = frameToProject + lookdir * timeToFrame * velocity
    //time to frame sollte runter skallieren oder? / sollte so stimmen...
    FVector offsetFuture = lookdirection * (timeToFrame * velocity);
    frameInWorld += offsetFuture;



    //EXTRA OFFSET NEEDED HERE, sont terrain nicht berührt, könnte durchlaufen!
	frameInWorld += FVector(0, 0, 200);

	//project frame to floor
	FVector downVec(0, 0, -1);
	FVector hitpoint;

	bool wasHit = performRaycast(world, frameInWorld, downVec, hitpoint);
	if(wasHit){

        //hier den offset wieder abziehen sodass die lokale posiion stimmt
        //zukunft wieder weg rechnen / abziehen
        hitpoint -= offsetFuture;

        actorTransform.transformFromWorldToLocalCoordinates(hitpoint);
		offsetMade = hitpoint - frameToProject;
		frameToProject = hitpoint;
	}
}

