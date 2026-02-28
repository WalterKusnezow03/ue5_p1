#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "InteractionComponentHoveredCache.generated.h"

UCLASS()
class GAMECORE_API UInteractionComponentHoveredCache : public UObject {
    GENERATED_BODY()

private:
    static UInteractionComponentHoveredCache *instancePtr;
public:
    
    static void UpdateHovered(UWidgetComponent *inActor); //can also be nullptr.
    static bool IsHoveredWidgetComponent(UWidgetComponent *checkActor);


protected:
    virtual void BeginDestroy() override;

    
    static void CreateInstanceIfNeeded(UWidgetComponent *inActor);
    static void CreateInstanceIfNeeded(UWorld *world);

    UWidgetComponent *currentHoveredWidget = nullptr;
};
