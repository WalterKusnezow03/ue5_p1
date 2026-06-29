#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/GameStartRoom/TextBox/TextBoxWidget.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/derived/BorderInterfaceUtilButton.h"
#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"
#include "NNCommunicationPlugin/Communication/Connection/base/notify/NNPathFinderSocketMessageReceiver.h"

#include "NNTrainWidget.generated.h"

class AGameStartRoom;

UCLASS()
class P2_API UNNTrainWidget : 
public UUserUi3DBaseWidget, 
public IBaseUiInterface,
public INNPathFinderSocketMessageReceiver
{
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

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTextLossWidget();


    //override
    virtual void Tick(float DeltaTime) override {};
    virtual bool dispatchClick(const FVector2D &position) override;
    virtual bool dispatchHover(const FVector2D &position) override;
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() override { return true; };
    virtual UWidget *baseLayoutPointer() override { return this; };

    //remove hover on widget left
    virtual void removeHover() override;

    virtual void ReceiveMessage(FString message) override;

protected:
    AGameStartRoom *parent = nullptr;

    void Init();
    BorderInterfaceUtilButton playButtonUtil;

    void SetText(FString message);
    void SetTextLoss(FString message);
    UTextBlock *GetTextBlockWidget(UWidget *raw);

    bool ExtractProgress(const TArray<FString> &parts, FString &resultText);
    bool ExtractLoss(const TArray<FString> &parts, FString &result);

    void DecomposeIntoArray(FString message, TArray<FString> &outArray, FString targetSeperator);

    int FindIndexLowerCase(const TArray<FString> &parts, FString target);

    int FindDigitsNeeded(FString &lossString);
};