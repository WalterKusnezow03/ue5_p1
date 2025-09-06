#include "GcLauncher.h"

AGcLauncher *AGcLauncher::instancePtr = nullptr;



AGcLauncher *AGcLauncher::Instance(){
    return instancePtr;
}

AGcLauncher* AGcLauncher::MakeInstance(UWorld *world){
    if(world != nullptr){

        UClass *toSpawn = AGcLauncher::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams); 
            if(spawned){

                AGcLauncher *casted = Cast<AGcLauncher>(spawned);
                if(casted){
                    AGcLauncher::instancePtr = casted;
                    

                    //set world of gc for spawning stuff
                    casted->collection.SetWorld(world);
                }
            }
        }
    }
    return nullptr;
}


void AGcLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason){
    collection.Clear(); //detructor like
    AGcLauncher::instancePtr = nullptr;
    Super::EndPlay(EndPlayReason);
}
