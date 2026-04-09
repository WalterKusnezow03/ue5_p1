// Copyright Walter Kusnezow All Rights Reserved.
#include "EventListenerStringStateWidget.h"



/// actor and widget part removed:  event_payload  
int UEventListenerStringStateWidget::ExtractPayload(TArray<FString> &message){
    if(message.Num() > 1){

        FString payload = message[1];
        int index = ConvertToIndex(payload);

        if(EventName.Contains("eventParking")){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("UEventListenerIndexStateWidget::ReceiveEvent parking extracted %d from %s"),
                    index,
                    *payload
                )
            );
        }
        return index;
    }
    return -1;
}

/// payload structure: original:    actor_widget_event_payload, removed
/// actor and widget part removed:  event_payload    
int UEventListenerStringStateWidget::ConvertToIndex(const FString &payload){
    if (int32* Value = TranslationMap.Find(FName(payload)))
    {
        int32 Result = *Value;
        return Result;
    }
    return -1;
}
