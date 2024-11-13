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
    if(deltaTime >= timeToFrame){
        reached = true;
        deltaTime = 0.0f;
        return target;
    }

    deltaTime += DeltaTime;
    
    
    FVector connect = target - from; // AB = B - A  
    //gx = A + r (B - A)
    //FVector interpolated = from + skalar() * connect;

    FVector interpolated = TargetInterpolator::interpolation(from, target, skalar());

    //wenn die richtungs vektoren anti paralell zu einander liegen
    //kann ich prüfen ob mein punkt passiert wurde
    FVector dirToB = target - interpolated;
    float dotProduct = FVector::DotProduct(dirToB.GetSafeNormal(), connect.GetSafeNormal());
    
    //is tested
    //DebugHelper::showScreenMessage("dot product: ", dotProduct);
    if (dotProduct <= -0.99f)
    {
        DebugHelper::showScreenMessage("PASSED FRAME");
        // anti parellell
        reached = true;
        deltaTime = 0.0f;
        return interpolated;
    }

    if(FVector::Dist(interpolated, target) <= 1.0f){ //1cm
        //DebugHelper::showScreenMessage("switched because of distance");
        reached = true;
        deltaTime = 0.0f;
        return interpolated;
    }
    return interpolated;

}

float TargetInterpolator::skalar(){
    if(timeToFrame == 0){
        return 1;
    }

    float skal = deltaTime / timeToFrame;
    //DebugHelper::showScreenMessage("skalar interpolate ", skal);

    // Berechnet den Skalierungsfaktor `t`, der zwischen 0 und 1 liegt
    return skal; //t / 1 quasi.

}




FVector TargetInterpolator::interpolation(FVector fromIn, FVector toIn, float skalar){
    FVector connect = toIn - fromIn; // AB = B - A
    //gx = A + r (B - A)
    FVector interpolated = fromIn + skalar * connect;
    return interpolated;
}