#pragma once

#include "Blueprint/UserWidget.h"

#include "EventWidgetBase.generated.h"

/// @brief ----- BASE CLASS FOR EVENT BASED WIDGET - CHANGING APPERANCE BASED ON EVENTS ------
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class ANYMESHWIDGETPLUGIN_API UEventWidgetBase : public UUserWidget {
    GENERATED_BODY()

public:
    virtual void TickExternal(float deltatime);

    virtual void ReceiveEvent(TArray<FString> &message);

protected:
    virtual void BeginDestroy() override;

private:
    void HideAll();
};