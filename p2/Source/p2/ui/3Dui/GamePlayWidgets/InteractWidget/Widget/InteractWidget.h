#pragma once


#include "Blueprint/UserWidget.h"

#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/BorderInterfaceUtil.h"

#include "CoreMath/animation/timer/Timer.h"

#include "InteractWidget.generated.h"

class AInteractWidgetActor;
class UWidgetProgressQuadCircular;

UCLASS()
class P2_API UInteractWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    
    void SetParentActor(AInteractWidgetActor *parent);

    virtual bool dispatchClick(const FVector2D &pos) override;
    virtual bool dispatchHover(const FVector2D &pos) override;

    virtual void UpdateInteractKeyHoldState(bool holdDownTrue) override;

    //remove hover on widget left
    virtual void removeHover() override;

    virtual void Tick(float DeltaTime) override;
    
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() { return true; }
    virtual UWidget *baseLayoutPointer() { return this; };

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetBorderWidget();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetProgressWidget();

protected:
    void Init();

    void ResetCircleProgressIfStateChanged(bool holdDown);
    void SetCircleProgressFromTimer();
    void SetCircleProgress(float scalar);
    UWidgetProgressQuadCircular *GetProgressWidgetCasted();

    //tick slate widget wrapper base on circle widget
    void TickCircleWidgetBase(float deltaTime);

    //tick progress of circle if key held down
    void TryTickCircleProgress(float deltatime);

private:
    BorderInterfaceUtil borderInterface;
    AInteractWidgetActor *parentActorWidget = nullptr;

    //tick timer if held down, otherwise reset
    bool interactionKeyIsHoldDown = false;
    Timer interactTimer;
};
