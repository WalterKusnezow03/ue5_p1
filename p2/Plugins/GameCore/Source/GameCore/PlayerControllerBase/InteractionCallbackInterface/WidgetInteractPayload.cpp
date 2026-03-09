#include "WidgetInteractPayload.h"


void UWidgetInteractPayload::SetActor(AActor *inActor){
    actor = inActor;
}

AActor *UWidgetInteractPayload::GetActor() const {
    return actor;
}