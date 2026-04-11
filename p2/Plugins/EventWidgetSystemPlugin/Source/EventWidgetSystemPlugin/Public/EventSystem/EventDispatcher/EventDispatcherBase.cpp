// Copyright Walter Kusnezow All Rights Reserved.
#include "EventDispatcherBase.h"

#include "EventWidgetSystemPlugin/Public/EventSystem/EventEnum/StringConversion/StringParser.h"

AEventDispatcherBase *AEventDispatcherBase::instancePtr = nullptr;


AEventDispatcherBase::AEventDispatcherBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

AEventDispatcherBase *AEventDispatcherBase::Instance(){
    return instancePtr;
}

AEventDispatcherBase* AEventDispatcherBase::MakeInstance(UWorld *world){
    if(instancePtr){
        return instancePtr;
    }

    if(world != nullptr){

        UClass *toSpawn = AEventDispatcherBase::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams); 
            if(spawned){

                AEventDispatcherBase *casted = Cast<AEventDispatcherBase>(spawned);
                if(casted){
                    AEventDispatcherBase::instancePtr = casted;
                    return casted;
                }
            }
        }
    }
    return nullptr;
}


void AEventDispatcherBase::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //clean up!

    AEventDispatcherBase::instancePtr = nullptr;
    Super::EndPlay(EndPlayReason);
}



void AEventDispatcherBase::StaticReceiveEvent(FString message){
    if(AEventDispatcherBase *i = Instance()){
        i->ReceiveEvent(message);
    }
}


void AEventDispatcherBase::ReceiveEvent(FString message){

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("AEventDispatcherBase::ReceiveEvent %s"),
            *message
        )
    );

    //split up string
    StringParser parser;
    FString seperator = "_";
    TArray<FString> splitMessage = parser.Split(message, seperator);

    //construct widget key
    WidgetIdKey key;
    key.SetupFromFirstTwoStrings(splitMessage);
    ReceiveEvent(splitMessage, key);
}
void AEventDispatcherBase::ReceiveEvent(TArray<FString> &messageFull, WidgetIdKey &key){
    if(key.IsValidKey()){
        
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("AEventDispatcherBase::ReceiveEvent for %s"),
                *key.ToString()
            )
        );
        
        //remove id key from front of buffer
        if (messageFull.Num() >= 2){
            messageFull.RemoveAt(0, 2); //actorId_widgetId_(event)
            if(UEventWidgetBase *widget = FindWidget(key)){
                widget->ReceiveEvent(messageFull);
            }
        }
    }
}


UEventWidgetBase *AEventDispatcherBase::FindWidget(WidgetIdKey &key){
    if(registeredWidgets.find(key) != registeredWidgets.end()){
        return registeredWidgets[key];
    }
    return nullptr;
}

void AEventDispatcherBase::StaticRegister(
    const WidgetIdKey &key,
    UEventWidgetBase *widget
){
    if(AEventDispatcherBase* i = Instance()){
        i->Register(key, widget);
    }
}

void AEventDispatcherBase::StaticUnRegister(const WidgetIdKey &key){
    if(AEventDispatcherBase* i = Instance()){
        i->UnRegister(key);
    }
}

void AEventDispatcherBase::Register(
    const WidgetIdKey &key,
    UEventWidgetBase *widget
){
    if(!widget){
        return;
    }
    if(!key.IsValidKey()){
        return;
    }
    DebugHelper::logMessage("AEventDispatcherBase::Register Key ", key.ToString());

    registeredWidgets[key] = widget;

    //init on register (load animation for example)
    if(widget){
        widget->Init();
    }
}

void AEventDispatcherBase::UnRegister(const WidgetIdKey &key){
    if(!key.IsValidKey()){
        return;
    }
    registeredWidgets[key] = nullptr;
}


void AEventDispatcherBase::Tick(float deltatime){
    Super::Tick(deltatime);
    TickAllWidgets(deltatime);
    if(false){
        DebugHelper::showScreenMessage("AEventDispatcherBase::TICK", FColor::Red);
    }
}

void AEventDispatcherBase::TickAllWidgets(float deltatime){
    for(auto &pair : registeredWidgets){
        if(UEventWidgetBase *ptr = pair.second){
            ptr->TickExternal(deltatime);
        }
    }
}
