// Copyright Walter Kusnezow All Rights Reserved.
#include "EventListenerBaseWidget.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventBaseDispatcherWidget/EventWidgetBase.h"



void UEventListenerBaseWidget::TickExternal(float deltatime){
    Super::TickExternal(deltatime);
    if(CanTick()){
        //update here.
    }
}

/// payload structure: original:    actor_widget_event_payload, removed
/// actor and widget part removed:  event_payload    
void UEventListenerBaseWidget::ReceiveEvent(TArray<FString> &message){
    
    //is logged as expected
    //DebugHelper::logMessage("UEventListenerBaseWidget::ReceiveEvent ");

    //nothing yet in here, compare first string with own event
    if(IsTargetedByEvent(message)){
        logEventReceivalReconstructed(message);
        SetVisible(true);
        DebugHelper::logMessage("UEventListenerBaseWidget::ReceiveEvent SHOW");
    }else{
        SetVisible(false);
        DebugHelper::logMessage("UEventListenerBaseWidget::ReceiveEvent HIDE");
    }
}

void UEventListenerBaseWidget::logEventReceivalReconstructed(TArray<FString> &message){
    FString rebuild;
    for(FString &s : message){
        rebuild += "(";
        rebuild += s;
        rebuild += ")";

    }

    DebugHelper::logMessage("UEventListenerBaseWidget::ReceiveEvent SHOW", rebuild);
}



bool UEventListenerBaseWidget::IsTargetedByEvent(TArray<FString> &message){
    if(message.Num() > 0){
        FString event = message[0];
        if(event == UEventWidgetBase::forceEventString()){
            return true;
        }

        DebugHelper::logMessage(
            FString::Printf(TEXT("UEventListenerBaseWidget::ReceiveEvent Compare %s %s"), *message[0], *EventName)
        );
        return EventName.Contains(message[0]); //first string for event (actor_widget_event_payload)
    }
    return false;
}
