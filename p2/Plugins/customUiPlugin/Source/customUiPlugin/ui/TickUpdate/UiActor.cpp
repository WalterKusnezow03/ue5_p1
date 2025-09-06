#include "UiActor.h"

#include "GameCore/DebugHelper.h"
#include "customUiPlugin/ui/PlayerUiBase.h"

AUiActor *AUiActor::currentInstance = nullptr;

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
                //SET Player Ui INSTANCE to tick!!
                spawned->uiToUpdate = ptr; 

                currentInstance = spawned;
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


void AUiActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    currentInstance = nullptr;
    Super::EndPlay(EndPlayReason);
}

UWorld *AUiActor::GetWorldFromInstance(){
    if(currentInstance){
        return currentInstance->GetWorld();
    }
    return nullptr;
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