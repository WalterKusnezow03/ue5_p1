#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/GameStartRoom/TextBox/TextBoxWidget.h"

#include "WorldCreatorWidget.generated.h"

class AGameStartRoom;

UCLASS()
class P2_API UWorldCreatorWidget : public UUserWidget, public IBaseUiInterface{
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

    bool TextBoxIsActive();

protected:
    AGameStartRoom *parent = nullptr;

    UTextBoxWidget *GetTextBoxWidget();
};