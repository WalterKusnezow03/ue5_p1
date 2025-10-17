#include "DebugJointsActor.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "DebugPlugin/DebugHelper.h"
#include "Humanoid/IkSolver/NewtonSolve.h"

void ADebugJointsActor::CreateInstance(UWorld *world){
    if(world != nullptr){

        UClass *toSpawn = ADebugJointsActor::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            FVector location(0,0,0);
            ADebugJointsActor *spawned = world->SpawnActor<ADebugJointsActor>(
                toSpawn, 
                location, 
                FRotator::ZeroRotator,
                SpawnParams
            );
        }
    }
}


void ADebugJointsActor::BeginDestroy(){
    
    //delete created joints
    for (int i = 0; i < createdJoints.Num(); i++){
        Joint *current = createdJoints[i];
        delete current; 
    }
    Joint::resetId();
    Super::BeginDestroy();
    
}

ADebugJointsActor::ADebugJointsActor() : AActor(){
    PrimaryActorTick.bCanEverTick = true;
}

void ADebugJointsActor::BeginPlay(){
    Super::BeginPlay();
    initChain();
    timerFortick.Begin(3.0f, true); //auto reset
    target = FVector(100, 0, 0);

    Matrix3x3::testInverse();
}

void ADebugJointsActor::initChain(){
    TArray<FVector> lengths = {
        FVector(0, 0, -50),
        FVector(0, 0, -50),
        FVector(0, 0, -50),
    };
    Joint *parent = nullptr;
    for (int i = 0; i < lengths.Num(); i++)
    {
        FVector &currentTranslation = lengths[i];
        Joint *newJoint = new Joint(GetWorld(), currentTranslation);
        createdJoints.Add(newJoint);
        if(parent != nullptr){
            parent->SetChild(newJoint);
        }
        parent = newJoint;
    }

    testForwardTmpPluecker();
}

void ADebugJointsActor::Tick(float deltaTime){
    Super::Tick(deltaTime);

    MMatrix transform;
    FVector translation(1000, -1000, 300);
    transform.setTranslation(translation);

    timerFortick.Tick(deltaTime);
    debugSolveIk();

    if(createdJoints.Num() > 0){
        Joint *root = createdJoints[0];
        if (root != nullptr)
        {
            
            NewtonSolve solver;
            if(!solver.solutionIsAlreadyWithinRange(*root, target)){
                root->Tick(deltaTime, angularVelocity, linearVelocity);
            }

            root->Build(transform);

            //draw to root
            DebugHelper::showLineBetween(
                GetWorld(),
                FVector(0, 0, 0),
                translation,
                FColor::Green,
                deltaTime * 2.0f
            );

            //draw target
            DebugHelper::showLineBetween(
                GetWorld(),
                translation + target,
                translation,
                FColor::Yellow,
                deltaTime * 2.0f
            );

        }
    }
    
}








/**
 * IK by W testing
 */

void ADebugJointsActor::debugSolveIk(){

    if (timerFortick.timesUp() == false){
        return;
    }

    if(createdJoints.Num() > 0){
        Joint *ptr = createdJoints[0];

        target = FVector(100, 0, 0);
        NewtonSolve solver;
        if(ptr){
            if(solver.solutionIsAlreadyWithinRange(*ptr, target)){
                angularVelocity = FVector(0,0,0);
                return;
            }
            FVector wFound = solver.SolveForAngularVelocity(ptr, target, 3.0f); //3 sec wie timer
            if(solver.SolutionWasFound()){
                angularVelocity = wFound;
            }
        }

        /*
        if(ptr != nullptr){
            float deltaTime = 1.0f;
            MMatrix zeroTransform;
            FVector w(1.0f, 0.0f, 0.0f);
            FVector result = ptr->T_vonW(zeroTransform, w, deltaTime);
            DebugHelper::showScreenMessage("t(w) = ", result);
        }*/
    }   
}




void ADebugJointsActor::testForwardTmpPluecker(){
    if(createdJoints.Num() > 0){
        Joint *ptr = createdJoints[0];

        MMatrix transform;
        FVector a(100, 0, 0);
        for (int i = 0; i < 10; i++){
            a *= 2.0f;
            FVector result = ptr->T_vonW(
                transform,
                a,
                1.0f
            );
            //DebugHelper::logMessage("forward pluecker test ", result);
        }






    }
}