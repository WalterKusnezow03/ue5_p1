#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "WidgetActor.generated.h"

/// @brief will handle a UWidgetComponent as base, (3D World Widgets: Analogue to UCanvasRenderTarget and Textures and stuff.)
UCLASS()
class CUSTOMUIPLUGIN_API AWidgetActor : public AActor {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="LaunchProperties")
    bool CreateDefaultWidgetOnBeginPlay = false;

public:
    AWidgetActor();

    virtual void BeginPlay() override;

    virtual void Tick(float deltatime) override;

    ///@brief replaces the widget of the WidgetComponent, cant be nullptr.
    void ReplaceWidget(UUserWidget *widget);

protected:
    void Init();

    //must be a uuser widget, default UWidgets do not work!
    UPROPERTY()
    UUserWidget *internalWidget = nullptr;

    UPROPERTY()
    UWidgetComponent *WidgetComponent = nullptr;

private:



};