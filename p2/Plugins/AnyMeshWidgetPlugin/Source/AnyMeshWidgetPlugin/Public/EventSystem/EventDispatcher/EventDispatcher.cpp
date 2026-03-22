// Copyright Walter Kusnezow All Rights Reserved.
#include "EventDispatcher.h"

#include "AnyMeshWidgetPlugin/Public/EventSystem/EventEnum/StringConversion/StringParser.h"

AEventDispatcher *AEventDispatcher::instancePtr = nullptr;



AEventDispatcher *AEventDispatcher::Instance(){
    return instancePtr;
}

AEventDispatcher* AEventDispatcher::MakeInstance(UWorld *world){
    if(instancePtr){
        return instancePtr;
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


void AEventDispatcher::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //clean up!

    AEventDispatcher::instancePtr = nullptr;
    Super::EndPlay(EndPlayReason);
}



void AEventDispatcher::StaticReceiveEvent(FString message){
    if(AEventDispatcher *i = Instance()){
        i->ReceiveEvent(message);
    }
}


void AEventDispatcher::ReceiveEvent(FString message){

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("AEventDispatcher::ReceiveEvent %s"),
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
void AEventDispatcher::ReceiveEvent(TArray<FString> &messageFull, WidgetIdKey &key){
    if(key.IsValidKey()){
        
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("AEventDispatcher::ReceiveEvent for %s"),
                *key.ToString()
            )
        );
        
        //remove id key from front of buffer
        if (messageFull.Num() >= 2){
            messageFull.RemoveAt(0, 2);
            if(UEventWidgetBase *widget = FindWidget(key)){
                widget->ReceiveEvent(messageFull);
            }
        }
    }
}


UEventWidgetBase *AEventDispatcher::FindWidget(WidgetIdKey &key){
    if(registeredWidgets.find(key) != registeredWidgets.end()){
        return registeredWidgets[key];
    }
    return nullptr;
}

void AEventDispatcher::StaticRegister(const WidgetIdKey &key, UEventWidgetBase *widget, UWorld *world){
    if(AEventDispatcher* i = MakeInstance(world)){
        i->Register(key, widget);
    }
}

void AEventDispatcher::StaticUnRegister(const WidgetIdKey &key){
    if(AEventDispatcher* i = Instance()){
        i->UnRegister(key);
    }
}

void AEventDispatcher::Register(const WidgetIdKey &key, UEventWidgetBase *widget){
    if(!widget){
        return;
    }
    if(!key.IsValidKey()){
        return;
    }
    DebugHelper::logMessage("AEventDispatcher::Register Key ", key.ToString());

    registeredWidgets[key] = widget;

    //init on register (load animation for example)
    if(widget){
        widget->Init();
    }
}

void AEventDispatcher::UnRegister(const WidgetIdKey &key){
    if(!key.IsValidKey()){
        return;
    }
    registeredWidgets[key] = nullptr;
}