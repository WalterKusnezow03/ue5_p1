#include "TerrainLauncher.h"

ATerrainLauncher::ATerrainLauncher(){
    PrimaryActorTick.bCanEverTick = true; //needed for tick update
}

void ATerrainLauncher::makeInstance(UWorld *world, FString WorldLevelName){
    if(world != nullptr){

        UClass *toSpawn = ATerrainLauncher::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams); 
            if(spawned){

                ATerrainLauncher *casted = Cast<ATerrainLauncher>(spawned);
                if(casted){
                    casted->launch(WorldLevelName);
                }
            }
        }
    }
}

void ATerrainLauncher::launch(FString WorldLevelName){
    actorManager.BeginPlay(WorldLevelName, GetWorld());
}


void ATerrainLauncher::BeginPlay(){
    Super::BeginPlay();
    
}

// Override EndPlay
void ATerrainLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //actor manager save data
    actorManager.EndPlay();

    //super endplay
    Super::EndPlay(EndPlayReason);
}


void ATerrainLauncher::Tick(float deltatime){
    Super::Tick(deltatime);
    actorManager.Tick(deltatime);
}