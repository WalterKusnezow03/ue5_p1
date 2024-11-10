// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/TargetInterpolator.h"

TargetInterpolator::TargetInterpolator()
{
    reached = true;
    targetSetup = false;
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

    DebugHelper::showScreenMessage("override target, connect:", FColor::Red);
    target = totarget;
    
}





bool TargetInterpolator::hasReachedTarget(){
    return reached;
}


FVector TargetInterpolator::interpolate(float DeltaTime){
    if(reached){
        return target;
    }
    
    deltaTime += DeltaTime;
    
    FVector currentPos = from;
    FVector toFramePos = target;
    FVector connect = toFramePos - currentPos; // AB = B - A
    
    //gx = A + r (B - A)
    FVector interpolated = currentPos + skalar(timeToFrame) * connect;
    
    //wenn die richtungs vektoren anti paralell zu einander liegen
    //kann ich prüfen ob mein punkt passiert wurde
    FVector dirToB = toFramePos - interpolated;
    float dotProduct = FVector::DotProduct(dirToB.GetSafeNormal(), connect.GetSafeNormal());
    
    //is tested
    //DebugHelper::showScreenMessage("dot product: ", dotProduct);
    if (dotProduct <= -0.99f)
    {
        //anti parellell
        reached = true;
        deltaTime = 0.0f;
    }

    if(FVector::Dist(interpolated, toFramePos) <= 1.0f){ //20
        //DebugHelper::showScreenMessage("switched because of distance");
        reached = true;
        deltaTime = 0.0f;
    }
    return interpolated;

}

float TargetInterpolator::skalar(float timeDistance){
    if(timeDistance == 0){
        return 1;
    }

    float skal = deltaTime / timeDistance;
    //DebugHelper::showScreenMessage("skalar interpolate ", skal);

    // Berechnet den Skalierungsfaktor `t`, der zwischen 0 und 1 liegt
    return deltaTime / timeDistance; //t / 1 quasi.

}