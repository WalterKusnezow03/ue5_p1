// Copyright Walter Kusnezow All Rights Reserved.
#include "EventListenerNameComposedWidget.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventBaseDispatcherWidget/EventWidgetBase.h"


void UEventListenerNameComposedWidget::Init(){
    Super::Init();
    LoadListenerChildsOnInit();
}

void UEventListenerNameComposedWidget::LoadListenerChildsOnInit(){
    eventListenerChilds = TGetDirectChildren<UEventListenerBaseWidget>(); //from panel.
}

void UEventListenerNameComposedWidget::TickExternal(float deltatime){
    Super::TickExternal(deltatime);
    if(CanTick()){
        //not needed for now 
    }
}



/// payload structure: original:    actor_widget_event_payload, removed
/// actor and widget part removed:  event_payload    
void UEventListenerNameComposedWidget::ReceiveEvent(TArray<FString> &message){
    Super::ReceiveEvent(message); //shows / hides widget
    if(IsTargetedByEvent(message)){
        //event_payload
        //as
        //event_p1_p2_p3_p4...p_n
        for (int i = 1; i < message.Num(); i++){ //start at 1 since first is this event widget
            TArray<FString> generatedSubBuffer;
            MakeForcedEventMessage(message[i], generatedSubBuffer); //current payload attached
            
            //child index from 0
            int childIndex = i - 1;
            PushEventToChildAt(childIndex, generatedSubBuffer);
        }
    }
}

void UEventListenerNameComposedWidget::PushEventToChildAt(int index, TArray<FString> &message){
    if(index >= 0 && index < eventListenerChilds.Num()){
        if(UEventListenerBaseWidget *dispatched = eventListenerChilds[index]){
            dispatched->ReceiveEvent(message);
        }
    }
}



void UEventListenerNameComposedWidget::MakeForcedEventMessage(
    FString attachedPayload,
    TArray<FString> &outmessage
){
    outmessage.Add(UEventWidgetBase::forceEventString());
    outmessage.Add(attachedPayload);
}
