#include "TerrainLauncher.h"

ATerrainLauncher::ATerrainLauncher(){
    PrimaryActorTick.bCanEverTick = true; //needed for tick update
}

void ATerrainLauncher::makeInstance(UWorld *world){
    if(world != nullptr){

        UClass *toSpawn = ATerrainLauncher::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams);   
        }
    }
}

void ATerrainLauncher::BeginPlay(){
    Super::BeginPlay();
    FString worldLevelString = TEXT("World1");
    actorManager.BeginPlay(worldLevelString, GetWorld());
}

void ATerrainLauncher::Tick(float deltatime){
    Super::Tick(deltatime);
    actorManager.Tick(deltatime);
}