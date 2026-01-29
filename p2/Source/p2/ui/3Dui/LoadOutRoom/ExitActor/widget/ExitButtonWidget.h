#pragma once


#include "Blueprint/UserWidget.h"

#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/derived/BorderInterfaceUtilButton.h"

#include "ExitButtonWidget.generated.h"

class ALoadoutRoomExitButtonActor;

UCLASS()
class P2_API UExitButtonWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    
    void SetParentActor(ALoadoutRoomExitButtonActor *parent);

    virtual bool dispatchClick(const FVector2D &pos) override;
    virtual bool dispatchHover(const FVector2D &pos) override;

    virtual void Tick(float DeltaTime) override;
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() { return true; }
    virtual UWidget *baseLayoutPointer() { return this; };

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetBorderWidget();

protected:
    void Init();

private:
    BorderInterfaceUtilButton borderInterface;
    ALoadoutRoomExitButtonActor *parentActorWidget = nullptr;

};
