#include "SharedRaycastParamManager.h"

ASharedRaycastParamManager *ASharedRaycastParamManager::instance = nullptr;


void ASharedRaycastParamManager::MakeInstanceIfNeeded(AActor *actor){
    if(!instance && actor){
        UWorld *world = actor->GetWorld();
        MakeInstanceIfNeeded(world);
    }
}

void ASharedRaycastParamManager::MakeInstanceIfNeeded(UWorld *world){
    if(!instance && world){
        FActorSpawnParameters SpawnParams;
        FVector Location;
        ASharedRaycastParamManager *spawned = world->SpawnActor<ASharedRaycastParamManager>(
            ASharedRaycastParamManager::StaticClass(), 
            Location, 
            FRotator::ZeroRotator,
            SpawnParams
        ); 
        if(spawned){
            instance = spawned;
        }
    }
}

void ASharedRaycastParamManager::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //reset single ton
    if(instance){
        instance->sharedCollisionParamsProvider.Clear();
        instance = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}

/// @brief gets the collison params with ignored actor list internal from 
/// ASharedRaycastParamManager instance - is all skelletons tracked, so they dont ontop of each other.
/// carried items tracked too!
FCollisionQueryParams ASharedRaycastParamManager::getCollisonParams(){
    if(instance){
        return instance->sharedCollisionParamsProvider.getCollisonParams();
    }
    FCollisionQueryParams fallback;
    return fallback;
}

void ASharedRaycastParamManager::Add(AActor *actor){
    if(actor){
        MakeInstanceIfNeeded(actor); //make instance automatically, easier interface
        if(instance){
            instance->sharedCollisionParamsProvider.AddIgnoredActor(actor);
        }
    }
}
void ASharedRaycastParamManager::Remove(AActor *actor){
    if(actor){
        MakeInstanceIfNeeded(actor); //make instance automatically, easier interface
        if(instance){
            instance->sharedCollisionParamsProvider.RemoveIgnoredActor(actor);
        }
    }
}
