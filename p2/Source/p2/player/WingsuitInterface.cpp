// Fill out your copyright notice in the Description page of Project Settings.


#include "WingsuitInterface.h"
#include <cmath>
#include "DebugPlugin/DebugHelper.h"


WingsuitInterface::WingsuitInterface()
{
    world = nullptr;
    gravityDownAcceleration = -980.0f;
    wingsuitIsOpen = false;
    actorPointer = nullptr;
}

WingsuitInterface::~WingsuitInterface()
{
    world = nullptr;
    actorPointer = nullptr;
}

///@brief will return the gravity multiplier whether the wingsuit is open or not
float WingsuitInterface::currentGravityMultiplier(){
    if(wingsuitIsOpen){
        return 0.25f;
    }
    return 1.0f;
}

///@brief will return if the wingsuit is now open -> use for bone controller update!
bool WingsuitInterface::wingsuitIsOpenFlag(){
    return wingsuitIsOpen;
}




///@brief main tick function for the wingsuit interface 
void WingsuitInterface::Tick(
    UWorld *worldin,
    AActor &actor, 
    float DeltaTime
){
    actorPointer = &actor;
    actorLocation = actorPointer->GetActorLocation();
    world = worldin;
    /*if(world){
        gravityDownAcceleration = world->GetGravityZ();
    }*/

    TickWingsuitTimer(DeltaTime);
}


///@brief ticks the wingsuit timer and try to open / close the wingsuit on times up
/// with a new raycast
void WingsuitInterface::TickWingsuitTimer(float DeltaTime){
    if(wingsuitIsOpen || wingsuitJumpTriggered){
        wingsuitTimer.Tick(DeltaTime);
        if(wingsuitTimer.timesUp()){

            tryOpenWingsuit(); //check for closing wingsuit again

            wingsuitJumpTriggered = false;
        }
    }
    
    
}




///@brief has a higher priority of resetting the timer compared to "setWingsuitTimerOnMovement"
void WingsuitInterface::setWingsuitTimerOnJump(){
    if (!wingsuitIsOpen){
        wingsuitJumpTriggered = true; //check time anyways now 
        setWingsuitTimer(wingsuitUpdateInvertall / 2.0f);
    }
}

///@brief resets the wingsuit timer to a new time, which LOOPS
void WingsuitInterface::setWingsuitTimer(float time){
    wingsuitTimer.Begin(time, false); 
}




float WingsuitInterface::gravityCmsDown(){
    return gravityDownAcceleration * currentGravityMultiplier();
}

///@brief will try to open and close the wingsuit, based on distance to ground,
/// and sets the time for the
/// probably ground hit
void WingsuitInterface::tryOpenWingsuit(){
    DebugHelper::showScreenMessage("WINGSUIT CHECK", FColor::Cyan);

    float distanceFromGroundMeasured = 0.0f;
    if (isInAirRaycast(actorLocation, distanceFromGroundMeasured))
    {

        DebugHelper::showScreenMessage("WINGSUIT OPEN", FColor::Red);
        

        
        if(std::abs(v0) <= 0.1f){

            //x(t) = x0 + v0t + 1/2 a t^2
            //gesucht: next time gravity check
            //gesucht also t:

            //x(t) = distanceFromGround + 0*t + 1/2 a * t^2
            //x(t) = distanceFromGround + 1/2 a * t^2
            // 0 = distanceFromGround + 1/2 a * t^2
            // - distanceFromGround = 1/2 a * t^2 | * 2
            // - 2 * distanceFromGround = a * t^2 | :a
            // (-2 * distanceFromGround) / a = t^2 | sqrt
            // sqrt((-2 * distanceFromGround) / a) = t
            // t = sqrt(abs(distance * 2) / abs(gravity))

            float gravity = std::abs(gravityCmsDown());
            if(gravity > 0.01f){

                float nextTime = std::sqrt(
                    std::abs(distanceFromGroundMeasured * 2) /
                    gravity
                );
                
                changeGravityWingSuit();
                setWingsuitTimer(nextTime);
                DebugHelper::showScreenMessage("WINGSUIT OPEN: NEW TIME:", (float) nextTime, FColor::Red);

                //v(t) = v0 + at
                v0 += gravity * nextTime;
            }
        }else{

            //x(t) = distanceFromGround + vt + 1/2 a * t^2
            // 0 = 1/2 a * t^2 + vt + distanceFromGround | * 2
            // 0 = a * t^2 + 2*vt + 2*distanceFromGround | : a
            // 0 = t^2 + (2*vt)/a + (2*distanceFromGround)/a | ax^2 + px + q = 0, a = 1 !

            //(x0, x1) = -(p/2) +- sqrt((p/2)^2-q)

            // p = ((2*v)/a) / 2 <=> ((2*v)/a) * 1/2 <=> v / a
            // (x0, x1) = -(v / a) +- sqrt((v / a)^2 - ((2*distanceFromGround)/a))

            float gravity = std::abs(gravityCmsDown());
            if(gravity > 0.01f){
                float p2 = (v0 / gravity);

                float innerSqrt = (p2 * p2) - 2 * ((distanceFromGroundMeasured) / gravity);
                if (innerSqrt >= 0.0f) {
                    float nextTime = std::abs(-1 * p2 + std::sqrt(innerSqrt));

                    changeGravityWingSuit();
                    setWingsuitTimer(nextTime);
                    v0 += gravity * nextTime;

                    DebugHelper::showScreenMessage("WINGSUIT OPEN: NEW TIME:", (float) nextTime, FColor::Red);
                }else{
                    
                    changeGravityDefault();
                    v0 = 0.0f;

                }

                
            }
        }
        


    }
    else
    {
        DebugHelper::showScreenMessage("WINGSUIT CLOSE: distance:",distanceFromGroundMeasured, FColor::Orange);
        //boneController.closeWingsuit();
        changeGravityDefault();
        //setWingsuitTimer(wingsuitUpdateInvertall);
        v0 = 0.0f;
    }
}


void WingsuitInterface::changeGravityDefault(){
    wingsuitIsOpen = false;
}
void WingsuitInterface::changeGravityWingSuit(){
    wingsuitIsOpen = true;
}


bool WingsuitInterface::isInAirRaycast(FVector Start, float &distanceFromGround){
    

    if(world != nullptr){
        FVector End = Start + FVector(0,0,-1 * std::abs(minDistanceGroundForWingsuit)); //10 meter runter

        // Perform the raycast
        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(actorPointer); // Ignore the character itself
        Params.bTraceComplex = false; //better performance


        if(world){
            bool bHit = world->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
            if(!bHit){

                FVector hit = HitResult.ImpactPoint;
                float distOnZ = hit.Z - Start.Z;
                distanceFromGround = std::abs(distOnZ);

                return true;
            }
        }
    }
    
    
    return false;
}