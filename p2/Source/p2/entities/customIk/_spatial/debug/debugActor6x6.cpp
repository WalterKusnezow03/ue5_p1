
#include "CoreMinimal.h"
#include "p2/DebugHelper.h"
#include "p2/entities/customIk/_spatial/MMatrix6x6.h"
#include "debugActor6x6.h"

AdebugActor6x6::AdebugActor6x6(){

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


AdebugActor6x6 *AdebugActor6x6::Construct(UWorld *world){
    DebugHelper::logMessage("CONSTRUCT DEBUG ACTOR");
    if (world)
    {
        FVector location(0,0,0);


        FRotator rotation;
        FActorSpawnParameters params;
        AdebugActor6x6 *SpawnedActor = world->SpawnActor<AdebugActor6x6>(
            AdebugActor6x6::StaticClass(),
            location,
            FRotator::ZeroRotator,
            params
        );
        return SpawnedActor;
    }
    return nullptr;
}


void AdebugActor6x6::BeginPlay(){
    Super::BeginPlay();


    //matrizen translation und rotationen erstellen
    a.setTranslation(FVector(0, 0, 0));
    b.setTranslation(FVector(0, 0, -100));
    c.setTranslation(FVector(0, 0, -100));

    FRotator r;
    r.Pitch -= 20;
    b.setRotation(r);
}

void AdebugActor6x6::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    //DebugHelper::showScreenMessage("debug tick");
    debugBuild(DeltaTime);
}

void AdebugActor6x6::debugBuild(float DeltaTime){

    //build all matrices
    FVector6D initalJoint6D;
    FVector dir(100, 0, 100);
    initalJoint6D.updateLiniearVelocity(dir);

    std::vector<MMatrix6x6 *> matrices = {&a, &b, &c};
    FVector debugActor(1000, -1000, 200);


    DebugHelper::showLineBetween(GetWorld(), debugActor, FVector(0,0,0), FColor::Blue, DeltaTime * 2.0f);

    std::vector<FVector> outDraw;
    FVector6D propagated = initalJoint6D;
    
    //hier testen: erst multiplizieren alle und dann fvector builden.


    /*
    //iterarien und position updaten
    
    for (int i = 0; i < matrices.size(); i++)
    {
        MMatrix6x6 &current = *matrices[i];

        FVector6D newLocation = current * propagated;
        propagated = newLocation;

    
        FVector drawLocation = newLocation.extractPosition(debugActor, 1.0f);
        //FVector drawLocation = newLocation.extractPosition(debugActor, DeltaTime);

        outDraw.push_back(drawLocation);
    }*/


    //draw
    for (int i = 1; i < outDraw.size(); i++){
        FVector prev = outDraw[i - 1];
        FVector current = outDraw[i];

        DebugHelper::showLineBetween(GetWorld(), prev, current, FColor::Orange, DeltaTime * 2.0f);
    }
}