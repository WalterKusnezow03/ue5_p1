

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/PlayerUi.h"
#include "LoadoutScreen.h"

LoadoutScreen::LoadoutScreen(){

}

LoadoutScreen::LoadoutScreen(UPlayerUi &refin){
    saveParent(refin);
    createBaseCanvas();
}


LoadoutScreen::~LoadoutScreen(){

}