

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/PlayerUi.h"
#include "LoadoutScreen.h"

LoadoutScreen::LoadoutScreen(){
    baseCanvas = nullptr;
    backgroundBlur = nullptr;
}

LoadoutScreen::LoadoutScreen(UPlayerUi &refin){
    baseCanvas = nullptr;
    backgroundBlur = nullptr;

    playerUiParent = &refin;
    createBaseCanvas(); //super
    createBackgroundBlur(); 

    createSideMenu();
}

LoadoutScreen::~LoadoutScreen(){
    baseCanvas = nullptr;
    backgroundBlur = nullptr;
}


void LoadoutScreen::createSideMenu(){

}

void LoadoutScreen::createExitButton(){

}
