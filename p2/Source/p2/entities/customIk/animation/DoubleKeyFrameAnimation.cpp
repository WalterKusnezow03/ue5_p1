// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/UnrealMathUtility.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/entities/customIk/bonePackage/BoneControllerStates.h"
#include "DoubleKeyFrameAnimation.h"

DoubleKeyFrameAnimation::DoubleKeyFrameAnimation()
{
    deltaTime = 0.0f;
    isRunningAnimation = false;
}

DoubleKeyFrameAnimation::~DoubleKeyFrameAnimation()
{
}




void DoubleKeyFrameAnimation::setAnimationA(KeyFrameAnimation &&other){
    framesA = MoveTemp(other);
}

void DoubleKeyFrameAnimation::setAnimationBAdjustPermanentTarget(FVector vector){
    bTarget = vector;
    bIsSetToAutoOverride = true;
}

bool DoubleKeyFrameAnimation::isAnimationA(){
    return isAnimationAPlaying;
}

bool DoubleKeyFrameAnimation::isAnimationB(){
    return !isAnimationAPlaying;
}



/// @brief gibt den frame zureück der erreicht wurde von der letzen / vorherigen animation (FRAMES A)
/// dazu da um der hip zu sagen wo sie sich relativ befindet und wo sie sich bewegen soll!
/// @return frame reached in animation A, final leg pos made!
FVector DoubleKeyFrameAnimation::readPrevAnimationReachedFrame(){
    return aReachedTickFrame;
}

/// @brief interpolates A or B based on which animation is now wanted
/// @param DeltaTime delta time passed from Tick()
/// @return interpolated frame target
FVector DoubleKeyFrameAnimation::interpolate(float DeltaTime){
    
    deltaTime += DeltaTime;

    
    FVector interpolated;
    if(isAnimationA()){
        interpolated = framesA.interpolate(DeltaTime);

        DebugHelper::showScreenMessage("A interpolating !", interpolated, FColor::Cyan);

        if(framesA.reachedLastFrameOfAnimation()){
            aReachedTickFrame = interpolated;

            DebugHelper::showScreenMessage("A reached !", interpolated, FColor::Emerald);

            //intepolierte position relativ zur hüfte ist einfach den vektor
            //umzudrehen
            if(bIsSetToAutoOverride){
                
                //relativer frame vom fuss zur hip
                FVector relativeBFrame = aReachedTickFrame * -1; 


                //DEBUG HACKING TIME!
                //debug was 1.0f for walking
                float timeToFrameB = 0.1f; //muss später anhand actor velocity skalliert werden!
                if(!isRunningAnimation){
                    timeToFrameB = framesA.totalLength();
                    timeToFrameB = 0.5f; //debug
                }

                //FVector relativeBFrame = aReachedTickFrame * -1; 
                interpolateB.setTarget(relativeBFrame, bTarget, timeToFrameB);


                
            }
            isAnimationAPlaying = false;
        }


    }
    else{
        DebugHelper::showScreenMessage("B interpolating!", FColor::Black);
        interpolated = interpolateB.interpolate(DeltaTime);
        if(interpolateB.hasReachedTarget()){
            cycleComplete = true;
            projectionHipOffset = FVector(0, 0, 0);
            isAnimationAPlaying = true; //flip bool!
            deltaTime = 0.0f;
        }
    }


    return interpolated;
}





/// @brief will return if the animation cycle was complete
/// and reset the boolean if true
/// @return animation cycle was complete in the last interpolation (frame)
bool DoubleKeyFrameAnimation::animationCycleWasComplete(){
    if(cycleComplete){
        cycleComplete = false;
        return true;
    }
    return false;
}


/// @brief will return if the time needed is reached to finish an animation or interpolation
/// @param timeCheck 
/// @return 
bool DoubleKeyFrameAnimation::reachedTime(float timeCheck){
    if(isAnimationA()){
        return framesA.reachedLastFrameOfAnimation();
    }
    return timeCheck >= reachTime(); //interpolator B for now 

}

float DoubleKeyFrameAnimation::reachTime(){
    if(isAnimationA()){
        return framesA.totalLength();
    }else{
        return interpolateB.TimeToFrame();
    }
}













/// @brief will process the offset made from projection for hip adjustment 
///         -> use the getProjectionHipOffsetTimed() method to apply your hip offset over time
/// @param offsetMade 
void DoubleKeyFrameAnimation::processProjectOffset(FVector &offsetMade){
    
    //float timeCopy = framesA.totalLength() + interpolateB.TimeToFrame(); // do in total time (?)
    float timeCopy = interpolateB.TimeToFrame(); //testing needed for only during adjust time!

    float timeInFrames = timeCopy * 60.0f;
    if (timeInFrames > 0.1f){
        projectionHipOffset = offsetMade / timeInFrames; //60fps, nicht einfach teilen, durch zahl der frames!
    }else{
        projectionHipOffset = FVector(0,0,0); //no offset if not possible
    }

    //DebugHelper::showScreenMessage("processed offset: ", projectionHipOffset, FColor::Emerald);

    //projektion entfernen wenn man die treppe hochläuft, nur bei negativen werten!
    if(projectionHipOffset.Z > 0.0f){
        projectionHipOffset = FVector(0, 0, 0); //TESTING blocking pos offset
    }
}





/// @brief additional hip offset needed because of projected frames, will be (0,0,0) if
/// no offset needed, or any other vector if needed.
/// @return vector offset for hip to add, is timed with animation linear.
FVector DoubleKeyFrameAnimation::getProjectionHipOffsetTimed(){
    /**
     * only return if hip adjust time!
     * process of leg on floor and hip adjust
     */
    if(isAnimationB()){
        return projectionHipOffset;
    }


    //NEU: Flugphase wenn Anim A und running
    FVector flyingOffsetIfRunning = flyingOffset();
    return flyingOffsetIfRunning;
    //return FVector(0, 0, 0);
}











void DoubleKeyFrameAnimation::forceProjectNextFrame(
    UWorld *world, 
    MMatrix &actorMatrix
){
    if(world != nullptr){
        FVector offsetMade(0,0,0);
        framesA.forceProjectToGround(world, actorMatrix, offsetMade);
        //processProjectOffset(offsetMade);
    }
}


void DoubleKeyFrameAnimation::overrideCurrentStartingFrame(FVector &currentLocationRelative){
    framesA.overrideCurrentStartingFrame(currentLocationRelative); //override target interpolator start
}

void DoubleKeyFrameAnimation::forceOverrideNextFrame(FVector &pos){
    framesA.overrideNextFrameAndResetTime(pos);
}

/// @brief will skip the next A frames animation once completly with an start and end point!
/// @param start starting pos
/// @param end ending pos
void DoubleKeyFrameAnimation::skipAnimationOnce(FVector start, FVector end){
    framesA.skipAnimationOnce(start, end);
}






/// @brief switch from default walking mode to running mode
/// @param b enable or disable running mode for flying offset
void DoubleKeyFrameAnimation::setRunning(bool b){
    isRunningAnimation = b;
}




/// @brief will create the current flying offset if running
/// @return flying offset 
FVector DoubleKeyFrameAnimation::flyingOffset(){
    if(isAnimationA() && isRunningAnimation){
        float totalTime = framesA.totalLength();
        return FVector(
            0,
            0,
            0 * sinusFlyingOffset(deltaTime, totalTime) //ist noch verbuggt, actor zu weit in der luft
        );
    }
    return FVector(0, 0, 0); //kein sprung ansonsten
}



float DoubleKeyFrameAnimation::sinusFlyingOffset(float time, float width){

    float slice = (PI / width); //Pi ist die zweite nullstelle
    float scaleDownFactor = 0.5f; //höhe skallieren

    float direction = time > (width * 0.5f) ? -1.0f : 1.0f; //nach der hälfte gehts runter

    return std::sin(slice * time) * direction * scaleDownFactor;
}







/**
 * 
 * 
 * --- PROJECTION SECTION ---
 * 
 * 
 */


void DoubleKeyFrameAnimation::projectNextFrameIfNeeded(
    UWorld *world, 
    MMatrix &actorMatrix
){
    bool b = false;
    float infheight = 99999.0f;
    float velocity = 0.0f;
    FVector noneDir(0, 0, 0);
    projectNextFrameIfNeeded(
        world, 
        actorMatrix, 
        velocity, 
        noneDir, b, 
        infheight, 
        BoneControllerStates::none
    );
}



/// @brief will project the next frame to the floor 
/// @param world world to project in
/// @param actorMatrix actor matrix (where the actor is / animated part)
/// @param velocity velocity, can be 0.0f
/// @param lookdir look dir of velocity, distance will not be affected if velocity is 0.0 for example
/// @param switchToArmLocomotion need to switch to arm locomotion output if maxHeightSwitch exceeded
/// @param maxHeightSwitch max height before switching locomotion boolean
/// @param locomotionType if the locomotion type is climbing, none hip extra offset will be saved
/// because the arm is moving differently from a leg / hip
void DoubleKeyFrameAnimation::projectNextFrameIfNeeded(
    UWorld *world,
    MMatrix actorMatrix, //is value pass on purpose
    float velocity,
    FVector &lookdir,
    bool &switchToArmLocomotion,
    float maxHeightSwitch,
    BoneControllerStates locomotionType
){

    if(world != nullptr){

        FVector removeFlyOffset = flyingOffset();
        actorMatrix -= removeFlyOffset;

        FVector offsetMade(0,0,0);
        bool wasProjected = framesA.projectNextFrameToGroundIfNeeded(
            world, 
            actorMatrix, 
            offsetMade,
            velocity,
            lookdir
        );
        if(offsetMade.Z > maxHeightSwitch){
            switchToArmLocomotion = true;
            offsetMade = FVector(0, 0, 0); //reset, switched to arm locomotion
        }
        if(locomotionType == BoneControllerStates::locomotionClimb){ //dont apply anything if climbing keys
            offsetMade = FVector(0, 0, 0);
        }

        if(wasProjected){
            processProjectOffset(offsetMade);
        }


        //hier zusätzlich neue velocity speichern
        velocityOfActor = velocity;
    }
}