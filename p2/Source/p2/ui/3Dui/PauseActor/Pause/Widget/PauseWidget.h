#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"


#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/derived/BorderInterfaceUtilButton.h"

#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"

#include "PauseWidget.generated.h"

class APauseRoomActor;

UCLASS()
class P2_API UPauseWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    void Init();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetLoadoutButton();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetExitButton();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetHomeButton();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetSettingsButton();

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
   
    APauseRoomActor *parentActor = nullptr;

    bool InBound(UWidget *widget, const FVector2D &screenPos);

    BorderInterfaceUtilButton loadoutButtonUtil;
    BorderInterfaceUtilButton exitButtonUtil;
    BorderInterfaceUtilButton homeButtonUtil;
    BorderInterfaceUtilButton settingsButtonUtil;
    
};
