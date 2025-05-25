#include "DebugJointsActor.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/DebugHelper.h"

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

    Super::BeginDestroy();
}

ADebugJointsActor::ADebugJointsActor() : AActor(){
    PrimaryActorTick.bCanEverTick = true;
}

void ADebugJointsActor::BeginPlay(){
    Super::BeginPlay();
    initChain();
    timerFortick.Begin(0.1f, true); //auto reset
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
}

void ADebugJointsActor::Tick(float deltaTime){
    Super::Tick(deltaTime);

    if(createdJoints.Num() > 0){
        Joint *root = createdJoints[0];
        if (root != nullptr)
        {

            float scalar = 1.0f;
            timerFortick.Tick(deltaTime);
            if(timerFortick.timesUp() == false){
                scalar = 0.0f;
            }

            FVector angularVelocity = scalar * FVector(1.0f, 1.0f, 0.0f);
            FVector linearVelocity = scalar * FVector(1.0f, 0.0f, 0.0f);

            root->Tick(deltaTime, angularVelocity, linearVelocity);
            // root->Tick(deltaTime, torque);

            MMatrix transform;
            FVector translation(1000, -1000, 300);
            transform.setTranslation(translation);
            root->Build(transform);

            DebugHelper::showLineBetween(
                GetWorld(),
                FVector(0, 0, 0),
                translation,
                FColor::Green,
                deltaTime * 2.0f
            );
        }
    }
    
}
