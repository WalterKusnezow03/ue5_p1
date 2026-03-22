#include "AnyMeshWidgetExtractEventCompatible.h"


void UAnyMeshWidgetExtractEventCompatible::BeginPlay(){
    Super::BeginPlay();
    RegisterWidgetToEventDispatcherOnBeginPlay();
}

void UAnyMeshWidgetExtractEventCompatible::RegisterWidgetToEventDispatcherOnBeginPlay(){
    WidgetIdKey key = MakeKey();
    AEventDispatcher::StaticRegister(key, TGetWidget<UEventWidgetBase>(), GetWorld());
}

void UAnyMeshWidgetExtractEventCompatible::EndPlay(const EEndPlayReason::Type EndPlayReason){
    UnRegisterWidgetToEventDispatcherOnEndPlay();
    Super::EndPlay(EndPlayReason);
}

void UAnyMeshWidgetExtractEventCompatible::UnRegisterWidgetToEventDispatcherOnEndPlay(){
    WidgetIdKey key = MakeKey();
    AEventDispatcher::StaticUnRegister(key);
}

WidgetIdKey UAnyMeshWidgetExtractEventCompatible::MakeKey(){
    WidgetIdKey key;
    key.Setup(actorName, widgetName);
    return key;
}
