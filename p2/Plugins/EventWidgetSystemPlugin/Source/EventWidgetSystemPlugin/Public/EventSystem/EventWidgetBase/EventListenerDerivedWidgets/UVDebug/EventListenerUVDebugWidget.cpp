#include "EventListenerUVDebugWidget.h"
#include "DebugPlugin/DebugHelper.h"

void UEventListenerUVDebugWidget::Init(){
    Super::Init();
    SetVisible(false);
}

void UEventListenerUVDebugWidget::ReceiveEvent(TArray<FString> &message){
    //Super::ReceiveEvent(message);
    if(message.Num() > 1){
        FString firstString = message[0];
        FString secondString = message[1];
        if(firstString.Contains("UVEvent")){

            if(secondString.Contains("Enabled")){
                SetVisible(true);
            }
            if(secondString.Contains("Disabled")){
                SetVisible(false);
            }

            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("UEventListenerUVDebugWidget::%s_%s"),
                    *firstString,
                    *secondString
                )
            );
            
        }

    }
}