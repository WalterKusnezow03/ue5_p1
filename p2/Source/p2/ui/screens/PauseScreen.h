#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "CanvasScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"

class UPlayerUi;

class P2_API PauseScreen : public CanvasScreen {

public:
    PauseScreen();
    PauseScreen(UPlayerUi &owningParent);
    ~PauseScreen();

private:
    UCanvasPanel *baseCanvas = nullptr;
    UVerticalBox *menu = nullptr;

    void createBaseCanvas();
    void createMenu();
};