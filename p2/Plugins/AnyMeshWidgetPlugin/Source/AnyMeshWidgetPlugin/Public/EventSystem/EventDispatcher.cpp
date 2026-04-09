// Copyright Walter Kusnezow All Rights Reserved.
#include "EventDispatcher.h"

AEventDispatcher *AEventDispatcher::InstanceDerived(){
    if(AEventDispatcher *casted = Cast<AEventDispatcher>(Instance())){
        return casted;
    }
    return nullptr;
}

AEventDispatcher* AEventDispatcher::MakeInstanceDerived(UWorld *world){
    if(AEventDispatcher *a = InstanceDerived()){
        return a;
    }

    if(world != nullptr){

        UClass *toSpawn = AEventDispatcher::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams); 
            if(spawned){

                AEventDispatcher *casted = Cast<AEventDispatcher>(spawned);
                if(casted){
                    AEventDispatcher::instancePtr = casted;
                    return casted;
                }
            }
        }
    }
    return nullptr;
}





void AEventDispatcher::StaticRegisterComponent(
    const WidgetIdKey &key, 
    UAnyMeshWidgetExtractEventCompatible *widgetComponent
){
    if(AEventDispatcher* i = InstanceDerived()){
        i->RegisterComponent(key, widgetComponent);
    }
}

void AEventDispatcher::RegisterComponent(
    const WidgetIdKey &key, 
    UAnyMeshWidgetExtractEventCompatible *widgetComponent
){
    if(!widgetComponent){
        return;
    }
    if(!key.IsValidKey()){
        return;
    }
    DebugHelper::logMessage("AEventDispatcher::Register Key ", key.ToString());

    registeredWidgetComponents[key] = widgetComponent;

}

void AEventDispatcher::UnRegister(const WidgetIdKey &key){
    if(!key.IsValidKey()){
        return;
    }
    Super::UnRegister(key); //unregister widget
    registeredWidgetComponents[key] = nullptr;
}

void AEventDispatcher::StaticFireColoredUVMapEvent(bool newFlag){
    if(AEventDispatcher *current = InstanceDerived()){
        current->FireColoredUVMapEvent(newFlag);
    }
}


void AEventDispatcher::FireColoredUVMapEvent(bool newFlag){
    if(newFlag != uvColorCacheFlag){
        uvColorCacheFlag = newFlag;
        RebuildWidgetMeshData();

        FString event = "UVEvent";
        FString enabled = uvColorCacheFlag ? "Enabled" : "Disabled";

        TArray<FString> splitMessage = {event, enabled};

        for(auto &pair : registeredWidgets){
            if(UEventWidgetBase *widget = pair.second){
                widget->ReceiveEvent(splitMessage);
            }
        }
    }
}

void AEventDispatcher::RebuildWidgetMeshData(){
    for(auto &pair : registeredWidgetComponents){
        if(UAnyMeshWidgetExtractEventCompatible *widgetComponent = pair.second){
            widgetComponent->FlagMeshDataDirty();
        }
    }
}
