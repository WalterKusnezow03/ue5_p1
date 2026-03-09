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
    timerFortick.Begin(1.0f, true); //auto reset
    
    Matrix3x3::testInverse();
}

void ADebugJointsActor::initChain(){
    
    //Change debug chain for new chain
    Joint j3(FVector(0, 0, -50), GetWorld());
    Joint j2(FVector(0, 0, -50), GetWorld());
    Joint j11(FVector(20, 0, -30), GetWorld());
    Joint j1(FVector(0, 0, -50), GetWorld());
    Joint j0(FVector(0, 0, -50), GetWorld());
    RootJoint jRoot(FVector(0, 0, -50), GetWorld());

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
    rootJoint.BuildParentingRecursive();

    rootJoint.SetDrawingEnabledRecursive(true);
    rootJoint.SetDrawColorRecursive(FColor::Red, FColor::Cyan, 0);

    FVector translation(1000, -1000, 300);
    rootJoint.OverrideWorldLocation(translation);


    if(false){
        LockPosition();
    }
}

void ADebugJointsActor::LockPosition(){
    FJointConstraint &constraint = rootJoint.GetConstraint();
    constraint.LockPositionConstraint();
}

void ADebugJointsActor::Tick(float deltaTime){
    Super::Tick(deltaTime);

    if(!isEnabled){
        return;
    }
    timerFortick.Tick(deltaTime);
    TickDebugRandomTorque(deltaTime);
}


void ADebugJointsActor::TickDebugRandomTorque(float deltaTime){
    FVector angular(2, 1, 0); //x and y are logically flipped
    FVector linear(0, 0, 0);

    if(timerFortick.timesUp()){
        //angular = FVector(0, 0, 0);
        FVector force(3, 4, 1);
        rootJoint.AddForce(force, 10.0f);

        float time = 1.0f;
        timerFortick.Begin(time);
    }

    rootJoint.TickAndBuildRecursive(deltaTime);
    rootJoint.DrawJointLocation(deltaTime);

}



