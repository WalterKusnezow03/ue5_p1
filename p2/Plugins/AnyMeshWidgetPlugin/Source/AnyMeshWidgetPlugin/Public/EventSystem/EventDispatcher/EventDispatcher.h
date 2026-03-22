// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "AnyMeshWidgetPlugin/Public/EventSystem/EventDispatcher/WidgetIdKey.h"
#include "AnyMeshWidgetPlugin/Public/EventSystem/EventWidgetBase/EventBaseDispatcherWidget/EventWidgetBase.h"
#include <map>


#include "EventDispatcher.generated.h"


/// @brief widgets can be registered via AActor And widget name, and
/// events dispatched accordingly to registered widgets.
UCLASS()
class ANYMESHWIDGETPLUGIN_API AEventDispatcher : public AActor {
    GENERATED_BODY()
protected:
    static AEventDispatcher *instancePtr;

public:
    static AEventDispatcher *Instance();
    static AEventDispatcher *MakeInstance(UWorld *world);

    static void StaticReceiveEvent(FString message);

    static void StaticRegister(const WidgetIdKey &key, UEventWidgetBase *widget, UWorld *world);
    static void StaticUnRegister(const WidgetIdKey &key);

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void ReceiveEvent(FString message);
    void ReceiveEvent(TArray<FString> &messageFull, WidgetIdKey &key);

    void Register(const WidgetIdKey &key, UEventWidgetBase *widget);
    void UnRegister(const WidgetIdKey &key);

    std::map<WidgetIdKey, UEventWidgetBase *> registeredWidgets;

    UEventWidgetBase *FindWidget(WidgetIdKey &key);

    // -- event receiver --
    


};

