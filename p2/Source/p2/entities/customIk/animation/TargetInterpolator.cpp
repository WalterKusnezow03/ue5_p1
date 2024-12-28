// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/TargetInterpolator.h"

TargetInterpolator::TargetInterpolator()
{
    reached = true;
    targetSetup = false;
    timeToFrame = 0.0f;
}

TargetInterpolator::~TargetInterpolator()
{
}


bool TargetInterpolator::hasTargetSetup(){
    return targetSetup;
}

void TargetInterpolator::setTarget(FVector fromIn, FVector totarget, float timeToFrameIn){
    from = fromIn;
    target = totarget;
    timeToFrame = timeToFrameIn;
    deltaTime = 0.0f;
    reached = false;
    targetSetup = true;
}

/// @brief override target of a RUNNING ANIMATION!
/// @param totarget 
void TargetInterpolator::overrideTarget(FVector totarget){

    //DebugHelper::showScreenMessage("override target, connect:", FColor::Red);
    target = totarget;
}

void TargetInterpolator::overrideStart(FVector fromtarget){
    from = fromtarget;
}




void TargetInterpolator::overrideStartSpeedRelative(FVector newStart){
    

    float distanceOld = FVector::Dist(from, target);
    float speed = distanceOld / timeToFrame; //sei distanz 40m und ttf 2s, dann sinds 20ms


    // Aktualisiere Startpunkt
    from = newStart;

    //delta time muss nicht resettet werden! Ansonsten bricked alles.

    // Neue Time-to-Frame berechnen
    float newDistance = FVector::Dist(from, target);
    float newTimeToFrame = newDistance / speed;

    // Neue deltaTime basierend auf dem alten Fortschritt
    timeToFrame = newTimeToFrame;

    if(timeToFrame < 0.01f){
        reached = true;
    }
}






void TargetInterpolator::resetDeltaTime(){
    deltaTime = 0.0f;
    reached = false;
}

void TargetInterpolator::setNewTimeToFrame(float time){
    timeToFrame = time;
    reached = false;
}


bool TargetInterpolator::hasReachedTarget(){
    return reached;
}


FVector TargetInterpolator::interpolate(float DeltaTime){
    if(reached){
        worldtargetSetup = false;
        return target;
    }
    if(deltaTime >= timeToFrame){
        reached = true;
        deltaTime = 0.0f;
        worldtargetSetup = false;
        return target;
    }

    deltaTime += DeltaTime;
    
    
    FVector connect = target - from; // AB = B - A  
    //gx = A + r (B - A)
    //FVector interpolated = from + skalar() * connect;

    float skalarCurrent = skalar();
    FVector interpolated = TargetInterpolator::interpolation(from, target, skalarCurrent);
    

    //wenn die richtungs vektoren anti paralell zu einander liegen
    //kann ich prüfen ob mein punkt passiert wurde
    FVector dirToB = target - interpolated;
    float dotProduct = FVector::DotProduct(dirToB.GetSafeNormal(), connect.GetSafeNormal());
    
    //is tested
    //DebugHelper::showScreenMessage("dot product: ", dotProduct);
    if (dotProduct < 0.0f)
    {
        DebugHelper::showScreenMessage("PASSED FRAME");
        // anti parellell
        reached = true;
        deltaTime = 0.0f;
        worldtargetSetup = false;
        return interpolated;
    }

   
    return interpolated;

}





float TargetInterpolator::skalar(){
    if(timeToFrame == 0){
        return 1;
    }

    float skal = deltaTime / timeToFrame; //fractionThis = this / all
    
    if(skal > 1.0f){
        skal = 1.0f;
    }
    if(skal < 0.0f){
        skal = 0.0f;
    }

    // Berechnet den Skalierungsfaktor `t`, der zwischen 0 und 1 liegt
    return skal; //t / 1 quasi.

}




FVector TargetInterpolator::interpolation(FVector fromIn, FVector toIn, float skalar){
    FVector connect = toIn - fromIn; // AB = B - A
    //gx = A + r (B - A)
    FVector interpolated = fromIn + skalar * connect;
    return interpolated;
}



float TargetInterpolator::TimeToFrame(){
    return timeToFrame;
}



FVector TargetInterpolator::readFromPosition(){
    return from;
}

FVector TargetInterpolator::readToPosition(){
    return target;
}








/** 
 * 
 * 
 * ---- WORLD SPACE SECTION ----
 * 
 * 
 * 
 * man könnte hier noch überlegen ob man start local und worldgleichzeitig baut
 * 
 * 
*/


void TargetInterpolator::overrideStartWorld(FVector targetIn){
    fromWorld = targetIn;
}

void TargetInterpolator::overrideTargetWorld(FVector targetIn){
    targetWorld = targetIn;
    worldtargetSetup = true;
}

/// @brief overrides the starting position of the interpolation in world position
/// if the world target is setup, the local target will be also updated, relative to the actor matrix transform
/// @param newStart new world frame start
/// @param actor actor with rotation and translation
void TargetInterpolator::overrideStartWorldSpeedRelative(FVector newStart, MMatrix &actor){

    MMatrix inverse = actor.jordanInverse();
    if (!worldtargetSetup)
    {
        //translate start to local and override start
        FVector startLocal = inverse * newStart;
        overrideStartSpeedRelative(startLocal);
        return;
    }else{
        //DebugHelper::showScreenMessage("OVERRIDE WORLD START TARGET");
        
        //translate start and target to local and override both, target must be available
        FVector targetLocalNew = inverse * targetWorld;
        overrideTarget(targetLocalNew);
        FVector startLocal = inverse * newStart;
        overrideStartSpeedRelative(startLocal);
        return;
    }
}



