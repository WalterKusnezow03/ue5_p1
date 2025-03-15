#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "CanvasScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"

#include "Components/VerticalBox.h"

class UPlayerUi;

class P2_API PauseScreen : public CanvasScreen {

public:
    PauseScreen();
    PauseScreen(UPlayerUi &owningParent);
    ~PauseScreen();

private:
    UVerticalBox *menu = nullptr;
    
    TextButton exitButton;
    TextButton loadoutScreenButton;

    void createMenu();
    void createButtons();
    void createExitButton();
    void createLoadoutScreenButton();

    void AddChildToMenu(customUiComponentBase &item);
};