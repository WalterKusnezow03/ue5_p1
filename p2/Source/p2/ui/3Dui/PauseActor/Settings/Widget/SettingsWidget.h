#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"


#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/derived/BorderInterfaceUtilButton.h"

#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"

#include "SettingsWidget.generated.h"

class APauseRoomActor;
class USettingsOptionWidget;

UCLASS()
class P2_API USettingsWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    void Init();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetExitButton();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetDebugOptionWidget();
    


    void SetParentActor(APauseRoomActor *parent);
    void ResetParentActor();

    virtual bool dispatchClick(const FVector2D &pos) override;
    virtual bool dispatchHover(const FVector2D &position) override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() { return true; }
    virtual UWidget *baseLayoutPointer() { return this; };

    //remove hover on widget left
    virtual void removeHover() override;


protected:

private:
    void InitAllOptions();

    APauseRoomActor *parentActor = nullptr;

    bool InBound(UWidget *widget, const FVector2D &screenPos);

    BorderInterfaceUtilButton exitButtonUtil;



protected:
    //derived hover

    //doesnt have to be overriden but can be.
    virtual TArray<IBaseUiInterface *> GetAllItemsForDispatch() override;
    TArray<USettingsOptionWidget *> GetAllSettingsOptions();
};
