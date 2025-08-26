#pragma once

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "p2/ui/Widgets/buttons/subtypes/ImageOverlayedButton.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"

//debug
#include "customUiPlugin/ui/Widgets/textBox/TextBoxBase.h"

#include "PauseScreen.generated.h"

class UPlayerUiBase;

UCLASS()
class P2_API UPauseScreen : public UCanvasScreen {

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    virtual void init(UPlayerUiBase &playerUiParentref) override;
    
    

private:
    UPROPERTY()
    UVbox *menu = nullptr;
    
    UPROPERTY()
    UTextButton *exitButton = nullptr;

    UPROPERTY()
    UTextButton *loadoutScreenButton = nullptr;

    void createMenu();
    void createButtons();
    void createExitButton();
    void createLoadoutScreenButton();


    // ---- DEBUG ----

    UPROPERTY()
    UTextBoxBase *debugTextBox = nullptr;
    void debugCreateTextBox();

    UPROPERTY()
    UTextButton *debugGameLaunchButton = nullptr;
    void debugCreateGameLaunchButton();
};