// Copyright Walter Kusnezow All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "AnyMeshWidgetPlugin/Public/MeshExtractedComponent/AnyMeshWidgetExtracted.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventDispatcher/WidgetIdKey.h"


#include "AnyMeshWidgetExtractEventCompatible.generated.h"

UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class ANYMESHWIDGETPLUGIN_API UAnyMeshWidgetExtractEventCompatible : public UAnyMeshWidgetExtracted {
    GENERATED_BODY()

public:
    


protected:
    virtual void BeginPlay() override;

    void RegisterWidgetToEventDispatcherOnBeginPlay();

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void UnRegisterWidgetToEventDispatcherOnEndPlay();

    WidgetIdKey MakeKey();
private:
    UPROPERTY(EditAnywhere, Category = "_EventOption")
    FString widgetName = "HeckWidget";

    UPROPERTY(EditAnywhere, Category = "_EventOption")
    FString actorName = "Actor47";

    
};