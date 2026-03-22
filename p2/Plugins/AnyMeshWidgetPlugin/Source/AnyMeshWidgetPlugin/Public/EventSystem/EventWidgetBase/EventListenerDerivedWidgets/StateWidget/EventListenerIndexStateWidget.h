// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "AnyMeshWidgetPlugin/Public/EventSystem/EventWidgetBase/EventListenerBaseWidget/EventListenerBaseWidget.h"
#include "EventListenerIndexStateWidget.generated.h"

/// @brief Will show a UWidget (for example a image) based on a index
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class ANYMESHWIDGETPLUGIN_API UEventListenerIndexStateWidget : public UEventListenerBaseWidget {
    GENERATED_BODY()

public:

    virtual void Init() override;
    virtual void TickExternal(float deltatime) override;

    virtual void ReceiveEvent(TArray<FString> &message) override;

protected:
    void LoadWidgetsOnInit();
   

    UPROPERTY()
    TArray<UWidget *> widgets;

    int currentindex = -1;

    int ExtractPayload(TArray<FString> &message);
    void UpdateShownIndex(int index);
};
