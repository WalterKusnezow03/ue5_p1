// Copyright Walter Kusnezow All Rights Reserved.
#include "EventWidgetBase.h"

#include "DebugPlugin/DebugHelper.h"

#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventListenerBaseWidget/EventListenerBaseWidget.h"


void UEventWidgetBase::BeginDestroy()
{
    //clean up

    Super::BeginDestroy();
}

void UEventWidgetBase::ReceiveEvent(TArray<FString> &message){
    //HideAll(); //not needed here!
    //LogReceivedEvent(message);

    // -- dispatch data to all listener widgets --
    TArray<UEventListenerBaseWidget *> widgets = TGetDirectChildren<UEventListenerBaseWidget>();
    for (int i = 0; i < widgets.Num(); i++){
        if(UEventListenerBaseWidget *current = widgets[i]){
            current->ReceiveEvent(message);
        }
    }

    //DebugHelper::logMessage("UEventWidget::ReceiveEvent dispatched: ", widgets.Num());
}

void UEventWidgetBase::LogReceivedEvent(TArray<FString> &message){
    FString m = "UEventWidgetBase::ReceiveEvent "; //is TESTED!
    for(FString s : message){
        m += s;
    }

    DebugHelper::logMessage(m);
}


void UEventWidgetBase::TickExternal(float deltatime){
    Super::TickExternal(deltatime);
}