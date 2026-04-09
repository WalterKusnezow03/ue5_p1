// Copyright Walter Kusnezow All Rights Reserved.
#include "EventSystemDebugger.h"
#include "AnyMeshWidgetPlugin/Public/ComponentSettings/SharedAnyMeshWidgetComponentSettings.h"
#include "AnyMeshWidgetPlugin/Public/EventSystem/EventDispatcher.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventDispatcher/EventDispatcherBase.h"

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
    FireColoredUVMapEvent();
}

void AEventSystemDebugger::FireColoredUVMapEvent(){
    AEventDispatcher::StaticFireColoredUVMapEvent(ASharedAnyMeshWidgetComponentSettings::BShowColoredUVMap());
}


void AEventSystemDebugger::FireDebugEventHaube(){
    if(false){
        return;
    }

    FString message = "m4_haubeWidget_";
    FString event = FString::Printf(TEXT("event%d"), index + 1);
    message += event;

    index++;
    index = index % 2; // 1 or 2

    AEventDispatcherBase::StaticReceiveEvent(message);
}


void AEventSystemDebugger::FireDebugEventHeck(){
    indexHeck++;
    indexHeck = indexHeck % 3;

    //random debug
    if(indexHeck == 0){
        FString message = FString::Printf(TEXT("m4_heckWidget_eventAmpel_%d"), ampelIndex);
        ampelIndex++;
        ampelIndex = ampelIndex % 3; // 1 or 2
        AEventDispatcherBase::StaticReceiveEvent(message);
        return;
    }
    
    
    
    if(indexHeck == 1){

        //debug
        TArray<FString> lanes = {
            "empty",
            "right",
            "left",
            "forward"
        };

        FString message = FString::Printf(TEXT("m4_heckWidget_eventNavlane"));
        int countLanes = 5;
        for (int i = 0; i < countLanes; i++){
            int32 navlaneIndex = FMath::RandRange(0, lanes.Num() -1); // 0 including 3
            FString nameLane = FString::Printf(TEXT("_%s"), *lanes[navlaneIndex]);
            message += nameLane;
        }
        AEventDispatcherBase::StaticReceiveEvent(message);
        return;
    }

    if(indexHeck == 2){
        //debug
        TArray<FString> states = {
            "default",
            "left",
            "right"
        };

        FString message = FString::Printf(TEXT("m4_heckWidget_eventParking"));
        int32 stateIndex = FMath::RandRange(0, states.Num()-1); // 0 including 3
        FString nameState = FString::Printf(TEXT("_%s"), *states[stateIndex]);
        message += nameState;


        AEventDispatcherBase::StaticReceiveEvent(message);
        return;
    }


}
