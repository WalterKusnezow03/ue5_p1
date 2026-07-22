#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "CoreMath/animation/timer/FlipTimer.h"

#include "NNConnectionStateWidget.generated.h"

class USizeBox;
class UCanvasPanel;


//owns all markers and actors can subsribe by type, and unsubscribe
UCLASS()
class P2_API UNNConnectionStateWidget : public UUserWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:

    /// -- ufunction getter --

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetConnected();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetDisconnected();



    // --- needed for update ---
    virtual void Tick(float DeltaTime) override;
    virtual bool dispatchClick(const FVector2D &position) override {return false;}

	//MUST BE OVERRIDEN!!! - UWidget derived can return itself.
	virtual UWidget *baseLayoutPointer() override {
        return this;
    }

protected:
    bool NNIsConnected();
    void UpdateConnectionState(float DeltaTime);
    void SetVisibleFor(UWidget *widget, bool visible);

    FlipTimer timer;
    bool timerWasSetup = false;
    void SetupTimer();
    void TickTimer(float DeltaTime, bool connected);
};