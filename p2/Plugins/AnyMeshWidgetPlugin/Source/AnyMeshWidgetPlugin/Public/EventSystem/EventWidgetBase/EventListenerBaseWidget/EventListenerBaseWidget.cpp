// Copyright Walter Kusnezow All Rights Reserved.
#include "EventListenerBaseWidget.h"


void UEventListenerBaseWidget::TickExternal(float deltatime){
    Super::TickExternal(deltatime);
    if(CanTick()){
        //update here.
    }
}

/// payload structure: original:    actor_widget_event_payload, removed
/// actor and widget part removed:  event_payload    
void UEventListenerBaseWidget::ReceiveEvent(TArray<FString> &message){
    
    DebugHelper::logMessage("UEventListenerBaseWidget::ReceiveEvent A");

    //nothing yet in here, compare first string with own event
    if(IsTargetedByEvent(message)){
        SetVisible(true);
        DebugHelper::logMessage("UEventListenerBaseWidget::ReceiveEvent SHOW");
    }else{
        SetVisible(false);
        DebugHelper::logMessage("UEventListenerBaseWidget::ReceiveEvent HIDE");
    }
}

bool UEventListenerBaseWidget::IsTargetedByEvent(TArray<FString> &message){
    if(message.Num() > 0){
        DebugHelper::logMessage(
            FString::Printf(TEXT("UEventListenerBaseWidget::ReceiveEvent Compare %s %s"), *message[0], *EventName)
        );
        return EventName.Contains(message[0]); //first string for event (actor_widget_event_payload)
    }
    return false;
}
