#include "EventWidgetBase.h"

#include "DebugPlugin/DebugHelper.h"


void UEventWidgetBase::BeginDestroy()
{
    //clean up

    Super::BeginDestroy();
}

void UEventWidgetBase::TickExternal(float deltatime){
    //tick update here
}

void UEventWidgetBase::ReceiveEvent(TArray<FString> &message){
    HideAll();
    LogReceivedEvent(message);

    // -- convert events to messages --
    
}

void UEventWidgetBase::LogReceivedEvent(TArray<FString> &message){
    FString m = "UEventWidgetBase::ReceiveEvent "; //is TESTED!
    for(FString s : message){
        m += s;
    }

    DebugHelper::logMessage(m);
}

void UEventWidgetBase::HideAll(){

}