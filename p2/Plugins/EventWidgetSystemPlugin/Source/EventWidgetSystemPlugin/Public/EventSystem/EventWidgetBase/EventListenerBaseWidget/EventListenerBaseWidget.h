// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "Blueprint/UserWidget.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/WidgetBase/WidgetBase.h"

#include "EventListenerBaseWidget.generated.h"

/// @brief 
/// ------ BASE CLASS FOR EVENT BASED WIDGET --> LISTEN FOR EVENTS ------
/// ------ ON OFF STATE ONLY ------
/// Hide or show / change internal state based on received event!
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class EVENTWIDGETSYSTEMPLUGIN_API UEventListenerBaseWidget : public UWidgetBase {
    GENERATED_BODY()

protected:
    virtual bool CanTick(){
        return bIsSetVisible;
    }

public:
    virtual void TickExternal(float deltatime) override;

    /// payload structure: original:    actor_widget_event_payload, removed
    /// actor and widget part removed:  event_payload    
    virtual void ReceiveEvent(TArray<FString> &message);


protected:
    UPROPERTY(EditAnywhere, Category = "_EventOption")
    FString EventName = "event1";

    bool IsTargetedByEvent(TArray<FString> &message);

    void logEventReceivalReconstructed(TArray<FString> &message);
};