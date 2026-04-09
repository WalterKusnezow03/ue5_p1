// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "EventWidgetSystemPlugin/Public/EventSystem/EventDispatcher/WidgetIdKey.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventBaseDispatcherWidget/EventWidgetBase.h"
#include <map>

#include "EventDispatcherBase.generated.h"


/// @brief widgets can be registered via AActor And widget name, and
/// events dispatched accordingly to registered widgets.
UCLASS()
class EVENTWIDGETSYSTEMPLUGIN_API AEventDispatcherBase : public AActor {
    GENERATED_BODY()
protected:
    static AEventDispatcherBase *instancePtr;

public:
    AEventDispatcherBase();

    static AEventDispatcherBase *Instance();
    static AEventDispatcherBase *MakeInstance(UWorld *world);

    static void StaticReceiveEvent(FString message);

    static void StaticRegister(
        const WidgetIdKey &key, 
        UEventWidgetBase *widget
    );
    static void StaticUnRegister(const WidgetIdKey &key);

protected:
    virtual void Tick(float deltatime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void ReceiveEvent(FString message);
    void ReceiveEvent(TArray<FString> &messageFull, WidgetIdKey &key);

    void Register(const WidgetIdKey &key, UEventWidgetBase *widget);
    
    virtual void UnRegister(const WidgetIdKey &key);

    void TickAllWidgets(float deltatime);

    std::map<WidgetIdKey, UEventWidgetBase *> registeredWidgets;

    UEventWidgetBase *FindWidget(WidgetIdKey &key);

};

