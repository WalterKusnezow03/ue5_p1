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
    
    static void UpdateHovered(UWidgetComponent *inComponent); //can also be nullptr.
    static bool IsHoveredWidgetComponent(UWidgetComponent *checkComponent);

    static void UpdateInteractKeyHoldFlag(bool holdKey);
    static bool IsInteractKeyHoldWidgetComponent(UWidgetComponent *checkComponent);


protected:
    virtual void BeginDestroy() override;

    
    static void CreateInstanceIfNeeded(UWidgetComponent *inComponent);
    static void CreateInstanceIfNeeded(UWorld *world);

    UWidgetComponent *currentHoveredWidget = nullptr;
    bool interactKeyHoldDownCache = false;
};
