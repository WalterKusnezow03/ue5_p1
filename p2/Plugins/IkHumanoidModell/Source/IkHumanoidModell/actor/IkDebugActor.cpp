#include "IkDebugActor.h"
#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/DebugHelper.h"


void AIkDebugActor::CreateInstance(UWorld *world){
    if(world != nullptr){

        UClass *toSpawn = AIkDebugActor::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            FVector location(0,0,0);
            AIkDebugActor *spawned = world->SpawnActor<AIkDebugActor>(
                toSpawn, 
                location, 
                FRotator::ZeroRotator,
                SpawnParams
            );
        }
    }
}



AIkDebugActor::AIkDebugActor(){
    PrimaryActorTick.bCanEverTick = true;
}

void AIkDebugActor::BeginPlay(){
    Super::BeginPlay();

    FVector startWorld(200, -400, 210);
    hipController.setLocation(startWorld);
    hipController.setup(GetWorld());

    bone.setup(50, 50, GetWorld());

    // x is forward
    start = FVector(-30, 40, -50);
    end = FVector(30, -20, -50);
    start1 = FVector(-30, 0, 50);
    end1 = FVector(0, 0, 50);

    time = 2.0f;
    resetInterpolators();
}

void AIkDebugActor::resetInterpolators(){
    // x is forward
    interpolator.setTarget(start, end, time); //reset
    interpolator2.setTarget(start1, end1, time); //reset
    interpolator.useHermiteSplineInterpolation(false);
    interpolator2.useHermiteSplineInterpolation(false);
}


void AIkDebugActor::Tick(float deltatime){
    Super::Tick(deltatime);
    hipController.Tick(deltatime);

    /*
    FVector target;
    if (!flipFlag)
    {
        target = interpolator.interpolate(deltatime);
        if(interpolator.hasReachedTarget()){
            resetInterpolators();
            flipFlag = !flipFlag;
        }
        FVector v(200, -500, 300);
        MMatrix world(v);
        bone.MoveToTarget(target, world, deltatime);

        DebugHelper::showScreenMessage("tick 1", target, FColor::Green);
    }else{
        target = interpolator2.interpolate(deltatime);
        if(interpolator2.hasReachedTarget()){
            resetInterpolators();
            flipFlag = !flipFlag;
        }
        bone.MoveToTargetInverse(target, deltatime);

        DebugHelper::showScreenMessage("tick 2", target, FColor::Cyan);
    }*/
}
