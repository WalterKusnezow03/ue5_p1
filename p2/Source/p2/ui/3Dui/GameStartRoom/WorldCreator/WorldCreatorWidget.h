#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/GameStartRoom/TextBox/TextBoxWidget.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/derived/BorderInterfaceUtilButton.h"
#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"

#include "WorldCreatorWidget.generated.h"

class AGameStartRoom;

UCLASS()
class P2_API UWorldCreatorWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    void ClearParent();
    void SetParent(AGameStartRoom *parentIn);
    void ResetParent();

    void NotifyLaunch();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTextWidget();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetPlayButton();


    //override
    virtual void Tick(float DeltaTime) override {};
    virtual bool dispatchClick(const FVector2D &position) override;
    virtual bool dispatchHover(const FVector2D &position) override;
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() override { return true; };
    virtual UWidget *baseLayoutPointer() override { return this; };

    virtual void dispatchUserInput(UserInput &input) override;

    bool TextBoxIsActive();

protected:
    AGameStartRoom *parent = nullptr;

    void Init();
    BorderInterfaceUtilButton playButtonUtil;

    UTextBoxWidget *GetTextBoxWidget();
};