
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "CanvasScreen.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"
#include "p2/DebugHelper.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"
#include <functional>
#include "p2/gamestart/assetEnums/textureEnum.h"
#include "PauseScreen.h"


PauseScreen::PauseScreen(){
    baseCanvas = nullptr;
    menu = nullptr;
    backgroundBlur = nullptr;
}

PauseScreen::PauseScreen(UPlayerUi &parentOwner){
    baseCanvas = nullptr;
    menu = nullptr;
    backgroundBlur = nullptr;

    playerUiParent = &parentOwner;
    createBaseCanvas(); //super
    createBackgroundBlur();

    createMenu();
    createButtons();
}

PauseScreen::~PauseScreen(){
    baseCanvas = nullptr;
    menu = nullptr;
    backgroundBlur = nullptr;
}



void PauseScreen::createMenu(){
    if(menu == nullptr && baseCanvas != nullptr){
        menu = NewObject<UVerticalBox>(playerUiParent);
        baseCanvas->AddChild(menu);

        UCanvasPanelSlot *slot = Cast<UCanvasPanelSlot>(menu->Slot);
        if(slot != nullptr){
            slot->SetPosition(FVector2D(100, 100)); //kleiner test, nach innen schieben
        }
    }
}


void PauseScreen::createButtons(){
    createExitButton();
    createLoadoutScreenButton();
}

void PauseScreen::createExitButton(){
    if(menu != nullptr && playerUiParent != nullptr){
        exitButton = TextButton(*playerUiParent);
        exitButton.setText("exit"); //exit

        //set callback here
        exitButton.SetCallBack(
            FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::PauseKeyPressed)
        );

        AddChildToMenu(exitButton);
    }
}


void PauseScreen::createLoadoutScreenButton(){
    if(menu != nullptr && playerUiParent != nullptr){
        loadoutScreenButton = TextButton(*playerUiParent);
        loadoutScreenButton.setText("Loadout"); //exit

        //set callback here
        loadoutScreenButton.SetCallBack(
            FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::openLoadoutScreen)
        );

        AddChildToMenu(loadoutScreenButton);
    }
}


///@brief adds a custom ui component to the menu.
void PauseScreen::AddChildToMenu(customUiComponentBase &item){
    if(menu != nullptr){
        UWidget *baseLayout = item.baseLayoutPointer();
        if(baseLayout != nullptr){
            menu->AddChildToVerticalBox(baseLayout);
        }
    }
}

