// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "EventWidgetSystemPlugin/Public/EventSystem/EventDispatcher/WidgetIdKey.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventBaseDispatcherWidget/EventWidgetBase.h"
#include <map>
//#include "AnyMeshWidgetPlugin/Public/Component/AnyMeshWidgetComponentBase.h"
#include "AnyMeshWidgetPlugin/Public/MeshExtractedComponent/EventSystemRegisteredComponent/AnyMeshWidgetExtractEventCompatible.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventDispatcher/EventDispatcherBase.h"


#include "EventDispatcher.generated.h"


/// @brief widgets can be registered via AActor And widget name, and
/// events dispatched accordingly to registered widgets.
UCLASS()
class ANYMESHWIDGETPLUGIN_API AEventDispatcher : public AEventDispatcherBase {
    GENERATED_BODY()
protected:
    
public:
    static AEventDispatcher *InstanceDerived();
    static AEventDispatcher *MakeInstanceDerived(UWorld *world);

    static void StaticReceiveEvent(FString message);

    static void StaticRegisterComponent(
        const WidgetIdKey &key, 
        UAnyMeshWidgetExtractEventCompatible *widgetComponent
    );

    static void StaticFireColoredUVMapEvent(bool flag);

protected:
    
    void RegisterComponent(
        const WidgetIdKey &key, 
        UAnyMeshWidgetExtractEventCompatible *widgetComponent
    );
    virtual void UnRegister(const WidgetIdKey &key) override;

    // -- event receiver --

    bool uvColorCacheFlag = false;
    void FireColoredUVMapEvent(bool flag);
    std::map<WidgetIdKey, UAnyMeshWidgetExtractEventCompatible *> registeredWidgetComponents;
    void RebuildWidgetMeshData();
};

