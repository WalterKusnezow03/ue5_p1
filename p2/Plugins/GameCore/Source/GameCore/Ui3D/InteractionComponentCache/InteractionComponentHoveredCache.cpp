#include "InteractionComponentHoveredCache.h"

UInteractionComponentHoveredCache *UInteractionComponentHoveredCache::instancePtr = nullptr;

void UInteractionComponentHoveredCache::BeginDestroy()
{
    Super::BeginDestroy();
    if(instancePtr == this){
        instancePtr = nullptr;
    }
}

void UInteractionComponentHoveredCache::CreateInstanceIfNeeded(UWidgetComponent *inWidgetComponent){
    if(inWidgetComponent){
        CreateInstanceIfNeeded(inWidgetComponent->GetWorld());
    }
}
void UInteractionComponentHoveredCache::CreateInstanceIfNeeded(UWorld *world){
    if(world && !instancePtr){
        instancePtr = NewObject<UInteractionComponentHoveredCache>(world);
    }
}




void UInteractionComponentHoveredCache::UpdateHovered(UWidgetComponent *inWidgetComponent){
    CreateInstanceIfNeeded(inWidgetComponent);
    if(instancePtr){
        instancePtr->currentHoveredWidget = inWidgetComponent;
    }
}

bool UInteractionComponentHoveredCache::IsHoveredWidgetComponent(UWidgetComponent *checkComponent){
    if(checkComponent){
        CreateInstanceIfNeeded(checkComponent);
        if(checkComponent == instancePtr->currentHoveredWidget){
            return true;
        }
    }
    return false;
}
