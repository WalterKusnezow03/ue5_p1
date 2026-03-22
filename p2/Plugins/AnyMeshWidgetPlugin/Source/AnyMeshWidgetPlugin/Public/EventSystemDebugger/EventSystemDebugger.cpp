// Copyright Walter Kusnezow All Rights Reserved.
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
        FireDebugEventHaube();
        FireDebugEventHeck();
        timer.Begin(intervall, true);
    }
    timer.Tick(deltatime);

}


#include "AnyMeshWidgetPlugin/Public/EventSystem/EventDispatcher/EventDispatcher.h"
void AEventSystemDebugger::FireDebugEventHaube(){
    if(false){
        return;
    }

    FString message = "m4_haubeWidget_";
    FString event = FString::Printf(TEXT("event%d"), index + 1);
    message += event;

    index++;
    index = index % 2; // 1 or 2

    AEventDispatcher::StaticReceiveEvent(message);
}


void AEventSystemDebugger::FireDebugEventHeck(){
    FString message = FString::Printf(TEXT("m4_heckWidget_eventAmpel_%d"), ampelIndex);

    ampelIndex++;
    ampelIndex = ampelIndex % 3; // 1 or 2

    AEventDispatcher::StaticReceiveEvent(message);
}
