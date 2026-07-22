#include "NNConnectionStateWidget.h"
#include "customuipluginbase/baseInterface/WidgetHelper.h"



bool UNNConnectionStateWidget::NNIsConnected(){
    return NNPathFinderExtensionApi::NNIsConnected();
}

void UNNConnectionStateWidget::SetupTimer(){
    if(!timerWasSetup){
        bool resetsItself = true;
        float time = 0.3f;
        timer.Begin(time, resetsItself);
        timerWasSetup = true;
    }
}

void UNNConnectionStateWidget::TickTimer(float DeltaTime, bool connected){
    if(!connected){
        bool finished = timer.TickWithTimesUpReset(DeltaTime);
        float scalar = timer.scalar();
        WidgetHelper::SetRenderOpacity(GetWidgetConnected(), scalar);
    }
}



void UNNConnectionStateWidget::Tick(float DeltaTime){
    SetupTimer();
    UpdateConnectionState(DeltaTime);
}

void UNNConnectionStateWidget::UpdateConnectionState(float DeltaTime){
    bool connected = NNIsConnected();
    TickTimer(DeltaTime, connected);
    SetVisibleFor(GetWidgetConnected(), connected);
    SetVisibleFor(GetWidgetDisconnected(), !connected);
}

void UNNConnectionStateWidget::SetVisibleFor(UWidget *widget, bool visible){
    if(widget){
        WidgetHelper::SetVisible(widget, visible);
        WidgetHelper::SetRenderOpacity(widget, 1.0f);
    }
}

