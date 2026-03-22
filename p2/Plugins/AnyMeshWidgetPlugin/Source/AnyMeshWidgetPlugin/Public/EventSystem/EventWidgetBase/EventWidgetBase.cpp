#include "EventWidgetBase.h"



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

    // -- convert events to messages --


}


void UEventWidgetBase::HideAll(){
    
}