
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "CanvasScreen.h"
#include "PauseScreen.h"


PauseScreen::PauseScreen(){
    baseCanvas = nullptr;
    menu = nullptr;
}

PauseScreen::PauseScreen(UPlayerUi &parentOwner){
    saveParent(parentOwner);
    CanvasScreen::createBaseCanvas();
    createMenu();
}

PauseScreen::~PauseScreen(){
    baseCanvas = nullptr;
    menu = nullptr;
}


void PauseScreen::createMenu(){
    if(menu == nullptr && baseCanvas != nullptr){
        menu = NewObject<UVerticalBox>(playerUiParent);
        baseCanvas->AddChild(menu);
    }
}

