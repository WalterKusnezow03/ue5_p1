// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventListenerBaseWidget/EventListenerBaseWidget.h"
#include "EventListenerNameComposedWidget.generated.h"

/// @brief Will dispatch the payload data
/// to a row of children as (event_payload)->(event_p1_p2_p3_p4...p_n)
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class EVENTWIDGETSYSTEMPLUGIN_API UEventListenerNameComposedWidget : public UEventListenerBaseWidget {
    GENERATED_BODY()

public:

    virtual void Init() override;
    virtual void TickExternal(float deltatime) override;

    virtual void ReceiveEvent(TArray<FString> &message) override;

protected:
    void LoadListenerChildsOnInit();
   
    UPROPERTY()
    TArray<UEventListenerBaseWidget *> eventListenerChilds;


    int ExtractPayload(TArray<FString> &message);
    void UpdateShownIndex(int index);

    void MakeForcedEventMessage(
        FString attachedPayload,
        TArray<FString> &outmessage
    );
    void PushEventToChildAt(int index, TArray<FString> &message);
};
