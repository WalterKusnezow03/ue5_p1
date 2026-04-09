// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "Blueprint/UserWidget.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/WidgetBase/WidgetBase.h"

#include "EventWidgetBase.generated.h"

/// @brief ------ BASE CLASS FOR EVENT BASED WIDGET (For A extracted surface!) ------
///        ------ CHANGING APPERANCE BASED ON EVENTS, doesnt do anything except 
/// dispatching events and hiding / showing widgets based on it! ------
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class EVENTWIDGETSYSTEMPLUGIN_API UEventWidgetBase : public UWidgetBase {
    GENERATED_BODY()

public:
    virtual void TickExternal(float deltatime) override;

    /// payload structure: original:    actor_widget_event_payload, removed
    /// actor and widget part removed:  event_payload    
    virtual void ReceiveEvent(TArray<FString> &message);

    //event name to force events on listeners!
    static FString forceEventString() { return TEXT("FORCEEVENT"); };


protected:
    virtual void BeginDestroy() override;

    

private:
    void LogReceivedEvent(TArray<FString> &array);
};