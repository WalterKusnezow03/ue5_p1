// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventListenerBaseWidget/EventListenerBaseWidget.h"
#include "EventListenerUVDebugWidget.generated.h"

UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class EVENTWIDGETSYSTEMPLUGIN_API UEventListenerUVDebugWidget : public UEventListenerBaseWidget {
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void ReceiveEvent(TArray<FString> &message) override;

protected:

    


};




