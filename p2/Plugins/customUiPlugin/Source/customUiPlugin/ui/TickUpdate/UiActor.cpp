#include "UiActor.h"

#include "GameCore/DebugHelper.h"
#include "customUiPlugin/ui/PlayerUiBase.h"


AUiActor *AUiActor::createInstance(UWorld *world, UPlayerUiBase *ptr){
    if(world){
        UClass *toSpawn = AUiActor::StaticClass();

        if(toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;
    
            // Spawn the actor
            FVector Location(0, 0, 0);
            AUiActor *spawned = world->SpawnActor<AUiActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams);
            if(spawned != nullptr){
                spawned->uiToUpdate = ptr; //SET TICKED INSTANCE!
                return spawned;
            }
        }
    }
    return nullptr;
}

AUiActor::AUiActor(){
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.SetTickFunctionEnable(true);//manual enable because isnt in scene
}

void AUiActor::BeginPlay(){
    Super::BeginPlay();
    
}

void AUiActor::Tick(float DeltaTime){
    Super::Tick(DeltaTime); // <- das fehlt bei dir

    //tick player ui timer based actions
    //customUiComponentTickHandler::Tick(DeltaTime); //DEPRECATED
    if(uiToUpdate){
        uiToUpdate->updateClickDispatch(); //VERY IMPORTANT
        uiToUpdate->Tick(DeltaTime); //VERY IMPORTANT
    }
}