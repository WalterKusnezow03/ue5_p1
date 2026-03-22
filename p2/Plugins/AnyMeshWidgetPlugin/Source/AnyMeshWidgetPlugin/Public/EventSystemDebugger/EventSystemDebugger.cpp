#include "EventSystemDebugger.h"


AEventSystemDebugger *AEventSystemDebugger::instancePtr = nullptr;

AEventSystemDebugger::AEventSystemDebugger() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

void AEventSystemDebugger::MakeInstance(UWorld *world){
    if(instancePtr){
        return;
    }

    if(world != nullptr){

        UClass *toSpawn = AEventSystemDebugger::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams); 
            if(spawned){

                AEventSystemDebugger *casted = Cast<AEventSystemDebugger>(spawned);
                if(casted){
                    AEventSystemDebugger::instancePtr = casted;

                }
            }
        }
    }
}

void AEventSystemDebugger::EndPlay(const EEndPlayReason::Type EndPlayReason){
    AEventSystemDebugger::instancePtr = nullptr;
    Super::EndPlay(EndPlayReason);
}

void AEventSystemDebugger::BeginPlay(){
    Super::BeginPlay();
    timer.Begin(intervall, true);
}

void AEventSystemDebugger::Tick(float deltatime){
    if(timer.timesUp()){
        FireDebugEvent();
        timer.Begin(intervall, true);
    }
    timer.Tick(deltatime);

}


#include "AnyMeshWidgetPlugin/Public/EventSystem/EventDispatcher/EventDispatcher.h"
void AEventSystemDebugger::FireDebugEvent(){

    FString message = "m4_heckWidget_someEvent47";
    AEventDispatcher::StaticReceiveEvent(message);
}
