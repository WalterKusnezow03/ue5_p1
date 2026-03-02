#include "DebugJointsActor.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "DebugPlugin/DebugHelper.h"


void ADebugJointsActor::CreateInstance(UWorld *world, FVector &location){
    if(world != nullptr){

        UClass *toSpawn = ADebugJointsActor::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            ADebugJointsActor *spawned = world->SpawnActor<ADebugJointsActor>(
                toSpawn, 
                location, 
                FRotator::ZeroRotator,
                SpawnParams
            );
        }
    }
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
    
    //Change debug chain for new chain
    Joint j3(FVector(0, 0, -50), GetWorld());
    Joint j2(FVector(0, 0, -50), GetWorld());
    Joint j11(FVector(20, 0, -30), GetWorld());
    Joint j1(FVector(0, 0, -50), GetWorld());
    Joint j0(FVector(0, 0, -50), GetWorld());
    Joint jRoot(FVector(0, 0, -50), GetWorld());

    //lower joints debug
    j2.AddChild(j3);

    //split
    j11.AddChild(j2);
    j1.AddChild(j2);

    //merge
    j0.AddChild(j11);
    j0.AddChild(j1);
    jRoot.AddChild(j0);

    rootJoint = jRoot;
    rootJoint.SetDrawColorRecursive(FColor::Red, FColor::Cyan, 0);

    /*
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
    }*/

}

void ADebugJointsActor::Tick(float deltaTime){
    Super::Tick(deltaTime);
    timerFortick.Tick(deltaTime);

    MMatrix transform;
    FVector translation(1000, -1000, 300);
    transform.setTranslation(translation);

    TickDebugRandomTorque(transform, deltaTime);
}


void ADebugJointsActor::TickDebugRandomTorque(MMatrix &transform, float deltaTime){
    FVector angular(0, 3, 4); //x and y are logically flipped
    FVector linear(0, 0, 0);

    if(timerFortick.timesUp()){
        angular = FVector(0, 0, 0);
    }

    //testing needed!
    rootJoint.TickAndBuildRecursive(
        deltaTime, //deltaTime * 0.0001f,
        angular,
        linear,
        transform
    );
    rootJoint.DrawJointLocation(deltaTime);

}



