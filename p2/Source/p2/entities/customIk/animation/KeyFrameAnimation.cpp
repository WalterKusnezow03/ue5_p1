// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/TargetInterpolator.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"

KeyFrameAnimation::KeyFrameAnimation()
{
    frameIndex = 0;
    nextFrameIndex = 1;
    totalLengthSave = 0.0f;
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




float KeyFrameAnimation::totalLength(){
    return totalLengthSave;
}





/// @brief returns if enough frames are available for an animation (at least 2)
/// @return has enough or not
bool KeyFrameAnimation::hasAnyFrames(){
    return frames.size() >= 2; //must be at least 2!
}



/// @brief returns the deltatime from lastframe (for example to adjust the hip with the same time)
/// @return time
float KeyFrameAnimation::lastDeltaTime(){
    return lastDeltatime;
}






FVector KeyFrameAnimation::interpolate(float DeltaTime){
    
    if(hasAnyFrames()){
        //DebugHelper::showScreenMessage("nextframe ", nextFrameIndex);
        if (interpolator.hasTargetSetup() == false)
        {
            updateFrameInterpolator();
        }

        FVector interpolated = interpolator.interpolate(DeltaTime);

        if(interpolator.hasReachedTarget()){    
            //frameIndex++;
            updateFrameIndex();
            updateFrameInterpolator();
        }

        return interpolated;
    }
    DebugHelper::showScreenMessage("not enough frames!");
    return FVector(0, 0, 0);
}   

void KeyFrameAnimation::updateFrameIndex(){
    frameIndex = (frameIndex + 1) % frames.size();
    nextFrameIndex = (frameIndex + 1) % frames.size();
}



void KeyFrameAnimation::updateFrameInterpolator(){
    KeyFrame &currentFrame = frames.at(frameIndex);
    KeyFrame &nextFrame = frames.at(nextFrameIndex);
    interpolator.setTarget(
        currentFrame.readposition(),
        nextFrame.readposition(),
        nextFrame.timeToFrame()
    );

    //copy next target for the actor to get to adjust hip
    //targetCopy = nextFrame.readposition();
    targetCopy = nextFrame.readposition();

    //reset frame projected status
    frameIsProjected = false;
}

/**
 * todo:
 * man muss den nöchsten animation frame abgreifen und ihn ggf auf den boden plotten
 * man muss aber den aktuellen target frame bearbeiten können /
 * bzw auf den boden prozezieren
 * 
 * damit diese klasse das nicht selber machen muss muss ich den frame abholbar machen
 * der actor selber muss ihn vom lokalen ins welt koordinaten system bringen
 * wenn das geschehen, welt position vom boden / terrain abreifen, 
 * dann überschreiben
 * 
 * Es wäre sinnvoll den FVector per REFERENZ (zu gefährlich, keine pointer verteilen!)
 * (oder lieber pointer damit man ihn nicht per value annimmt) 
 * raus zu geben um ihn sofort zu bearbeiten
 */

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

FVector KeyFrameAnimation::readPrevFrame(){
    return targetCopy;
}


/// @brief override the next frame / target value, animation is not overriden, individual to current next
/// frame!
/// @param framePos 
void KeyFrameAnimation::overrideNextFrame(FVector &framePos){
    //DebugHelper::showScreenMessage("override target!");
    interpolator.overrideTarget(framePos);
    targetCopy = framePos; //MUST BE UPDATED TOO!
    frameIsProjected = true;
}

bool KeyFrameAnimation::nextFrameIsProjected(){
    return frameIsProjected;
}

