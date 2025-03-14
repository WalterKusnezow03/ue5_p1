#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "CanvasScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/makeUWidgets/TextButton.h"
#include "Components/VerticalBox.h"

class UPlayerUi;

class P2_API PauseScreen : public CanvasScreen {

public:
    PauseScreen();
    PauseScreen(UPlayerUi &owningParent);
    ~PauseScreen();

private:
    UVerticalBox *menu = nullptr;
    UBackgroundBlur *backgroundBlur = nullptr;

    void createBackgroundBlur();
    void createMenu();
    void createButtons();
    void createExitButton();

    TextButton exitButton;


};