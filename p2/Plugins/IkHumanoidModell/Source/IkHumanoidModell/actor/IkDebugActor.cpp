#include "IkDebugActor.h"
#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "DebugPlugin/DebugHelper.h"


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
    time = -10.0f; //abwarten bevor irgendwas passiert

    BeginPlaySingleArm();
    BeginPlayHipController();
    BeginPlayHumanoidController();
}

void AIkDebugActor::BeginPlaySingleArm(){
    if(debugPart == EDebugPart::EDebugArm){
        bone.setup(50, 50, GetWorld());


        //depracated debug tools
        // x is forward
        start = FVector(-30, 40, -50);
        end = FVector(30, -20, -50);
        start1 = FVector(-30, 0, 50);
        end1 = FVector(0, 0, 50);

    }
    
    
    resetInterpolators();
}

void AIkDebugActor::BeginPlayHipController(){
    if(debugPart == EDebugPart::EDebugHipController){
        FVector startWorld(400, -400, 210);
        hipController.SetLocation(startWorld);

        float initRotation = -90; //180 //10
        hipController.forceYawAdd(initRotation);
        hipController.setup(GetWorld());
        hipController.setStateWalking();
    }
}

void AIkDebugActor::BeginPlayHumanoidController(){
    if(debugPart == EDebugPart::EDebugHumanoidController){
        humanController.defaultSetup(GetWorld());
        FVector startWorld(400, -400, 210);
        humanController.SetLocation(startWorld);
        humanController.setStateWalking();
    }
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

    if(debugPart == EDebugPart::EDebugHumanoidController){
        TickPlayHumanoidController(deltatime);
    }
    if(debugPart == EDebugPart::EDebugHipController){
        TickPlayHipController(deltatime);
    }
    if(debugPart == EDebugPart::EDebugArm){
        TickPlaySingleArm(deltatime);
    }
}




void AIkDebugActor::TickPlayHipController(float deltatime){
    hipController.Tick(deltatime);

    //debug rotation
    time += deltatime;
    if(time > 5.0f && true){
        time = 0.0f;
        hipController.setupRotationForNextStep(MMatrix::degToRadian(30.0f));
    }
}

void AIkDebugActor::TickPlayHumanoidController(float deltatime){
    humanController.Tick(deltatime);

    //debug rotation
    time += deltatime;
    if(time > 5.0f && true){
        time = 0.0f;
        humanController.setupRotationForNextStep(MMatrix::degToRadian(30.0f));
    }
}




void AIkDebugActor::TickPlaySingleArm(float deltatime){
    
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
    }
}