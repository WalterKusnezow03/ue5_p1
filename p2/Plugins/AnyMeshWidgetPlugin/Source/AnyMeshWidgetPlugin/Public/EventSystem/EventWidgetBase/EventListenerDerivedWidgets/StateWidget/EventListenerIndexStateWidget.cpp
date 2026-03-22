// Copyright Walter Kusnezow All Rights Reserved.
#include "EventListenerIndexStateWidget.h"



void UEventListenerIndexStateWidget::Init(){
    Super::Init();
    LoadWidgetsOnInit();
}

void UEventListenerIndexStateWidget::LoadWidgetsOnInit(){
    widgets = GetDirectChildren(); //from panel.
    UpdateShownIndex(0);
}

void UEventListenerIndexStateWidget::TickExternal(float deltatime){
    Super::TickExternal(deltatime);
    if(CanTick()){
        //not needed for now 
    }
}



/// payload structure: original:    actor_widget_event_payload, removed
/// actor and widget part removed:  event_payload    
void UEventListenerIndexStateWidget::ReceiveEvent(TArray<FString> &message){
    Super::ReceiveEvent(message); //shows / hides widget
    if(IsTargetedByEvent(message)){
        //event_payload
        UpdateShownIndex(ExtractPayload(message));
    }
}

/// actor and widget part removed:  event_payload  
int UEventListenerIndexStateWidget::ExtractPayload(TArray<FString> &message){
    if(message.Num() > 1){
        FString payload = message[1];
        // Method 2: Using FCString::Atoi (requires * operator)
        int32 value = FCString::Atoi(*payload);
        return value;
    }
    return -1;
}

void UEventListenerIndexStateWidget::UpdateShownIndex(int index){
    if(currentindex == index){
        return;
    }
    currentindex = index;
    for (int i = 0; i < widgets.Num(); i++){
        if(UWidget *widget = widgets[i]){
            bool show = i == currentindex;
            UWidgetBase::SetVisible(widget, show);
        }
    }
}
