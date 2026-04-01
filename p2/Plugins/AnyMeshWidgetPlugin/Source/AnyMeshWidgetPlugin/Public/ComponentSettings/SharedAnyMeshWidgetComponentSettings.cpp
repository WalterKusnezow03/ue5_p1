#include "SharedAnyMeshWidgetComponentSettings.h"

ASharedAnyMeshWidgetComponentSettings *ASharedAnyMeshWidgetComponentSettings::instancePtr = nullptr;

void ASharedAnyMeshWidgetComponentSettings::BeginPlay(){
    Super::BeginPlay();


}


void ASharedAnyMeshWidgetComponentSettings::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //reset single ton
    instancePtr = nullptr;
    Super::EndPlay(EndPlayReason);
}



bool ASharedAnyMeshWidgetComponentSettings::BShowColoredUVMap(){
    if(instancePtr){
        return instancePtr->bShowColoredUVMap;
    }
    return false;
}

void ASharedAnyMeshWidgetComponentSettings::SetShowColorMap(bool flag){
    if(instancePtr){
        instancePtr->bShowColoredUVMap = flag;
    }
}

void ASharedAnyMeshWidgetComponentSettings::MakeInstanceIfNeeded(UWorld *world){
    if(!instancePtr){
        MakeInstance(world);
    }
}


ASharedAnyMeshWidgetComponentSettings* ASharedAnyMeshWidgetComponentSettings::MakeInstance(UWorld* World)
{
    if(instancePtr){
        return instancePtr;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ASharedAnyMeshWidgetComponentSettings");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ASharedAnyMeshWidgetComponentSettings* Spawned = World->SpawnActor<ASharedAnyMeshWidgetComponentSettings>(
            ASharedAnyMeshWidgetComponentSettings::StaticClass(), Location, Rotation, SpawnParams
        );   
        instancePtr = Spawned;
        return Spawned;
    }
    return nullptr;
}