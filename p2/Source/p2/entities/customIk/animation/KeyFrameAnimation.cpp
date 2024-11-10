// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/TargetInterpolator.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"

KeyFrameAnimation::KeyFrameAnimation()
{
    frameIndex = 0;
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

/// @brief DEPRECATED !
/// @param DeltaTime 
/// @return 
FVector KeyFrameAnimation::interpolateOld(float DeltaTime){
    
    if(hasAnyFrames()){
        clampIndex();
        int i = frameIndex;
        int inext = nextFrameIndex();

        deltaTime += DeltaTime; //sagen wir abstand erstmal eine sekunde:  t element [0,1]

        KeyFrame &current = frames.at(i);
        KeyFrame &toFrame = frames.at(inext);

        FVector currentPos = current.readposition();
        FVector toFramePos = toFrame.readposition();
        FVector connect = toFramePos - currentPos; // AB = B - A
        FVector dir = connect; //will be scaled by time!
        //.GetSafeNormal();

        float t = deltaTime; //t will be element [0,1]
        //gx = A + r (B - A)
        //FVector interpolated = currentPos + t * dir;
        FVector interpolated = currentPos + skalar(toFrame.timeToFrame()) * dir;

        if(FVector::Dist(interpolated, toFramePos) <= 10){
            frameIndex++;
            clampIndex();
            lastDeltatime = deltaTime; //copy for adjust
            deltaTime = 0;

        }

        return interpolated;
    }


    return FVector(0, 0, 0);
}

//TODO: prüfen ob richtig!
float KeyFrameAnimation::skalar(float timeDistance){
    // Berechnet den Skalierungsfaktor `t`, der zwischen 0 und 1 liegt
    return deltaTime / timeDistance; //teil / voll

}




/// @brief returns if enough frames are available for an animation (at least 2)
/// @return has enough or not
bool KeyFrameAnimation::hasAnyFrames(){
    return frames.size() >= 2; //must be at least 2!
}

/// @brief clamps the frame index to a valid value if needed
void KeyFrameAnimation::clampIndex(){
    if(frameIndex >= frames.size() || frameIndex < 0){
        frameIndex = 0;
    }
}

/// @brief returns the next frame index looping
/// @return 
int KeyFrameAnimation::nextFrameIndex(){
    if (frameIndex >= frames.size() - 1 || frameIndex < 0){
        return 0;
    }
    return frameIndex + 1;
}

int KeyFrameAnimation::prevFrameIndex(){
    if (frameIndex >= frames.size() - 1){
        return 0;
    }

    int maxIndex = frames.size() - 1;
    if (frameIndex <= 0 && maxIndex >= 0)
    {
        return maxIndex;
    }

    return frameIndex - 1;
}



/// @brief returns the deltatime from lastframe (for example to adjust the hip with the same time)
/// @return time
float KeyFrameAnimation::lastDeltaTime(){
    return lastDeltatime;
}






FVector KeyFrameAnimation::interpolate(float DeltaTime){
    
    if(hasAnyFrames()){
        clampIndex();
        if(interpolator.hasTargetSetup() == false){
            updateFrameInterpolator();
        }

        FVector interpolated = interpolator.interpolate(DeltaTime);

        if(interpolator.hasReachedTarget()){    
            frameIndex++;
            updateFrameInterpolator();
            //DebugHelper::showScreenMessage("reached, update interpolator");
        }

        return interpolated;
    }
    DebugHelper::showScreenMessage("not enough frames!");
    return FVector(0, 0, 0);
}   

void KeyFrameAnimation::updateFrameInterpolator(){
    clampIndex();
    int nextI = nextFrameIndex();
    KeyFrame &firstFrame = frames.at(frameIndex);
    KeyFrame &nextFrame = frames.at(nextI);
    interpolator.setTarget(
        firstFrame.readposition(),
        nextFrame.readposition(),
        nextFrame.timeToFrame()
    );

    //copy next target for the actor to get to adjust hip
    targetCopy = nextFrame.readposition();

    //neu
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
        clampIndex();
        int nextI = nextFrameIndex();
        KeyFrame &nextFrame = frames.at(nextI);
        return nextFrame.mustBeGrounded();
    }
    return false;
}

FVector KeyFrameAnimation::readNextFrame(){
    if(hasAnyFrames()){
        clampIndex(); //for safety
        int nextI = nextFrameIndex();
        KeyFrame &nextFrame = frames.at(nextI);
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