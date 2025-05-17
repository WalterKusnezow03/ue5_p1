
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "CanvasScreen.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"
#include "GameCore/DebugHelper.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"
#include <functional>
#include "p2/gamestart/assetEnums/textureEnum.h"
#include "PauseScreen.h"



void UPauseScreen::init(UPlayerUi &playerUiParentref){
    Super::init(playerUiParentref);

    //createBackgroundBlur();
    createBackgroundBlurAndDefaultColor();
    createMenu();
    createButtons();
}




void UPauseScreen::createMenu(){
    if(menu == nullptr && baseCanvas != nullptr){
        menu = NewObject<UVerticalBox>(this);

        if(menu){
            baseCanvas->AddChild(menu);

            UCanvasPanelSlot *slot = Cast<UCanvasPanelSlot>(menu->Slot);
            if(slot != nullptr){
                slot->SetPosition(FVector2D(100, 100)); //kleiner test, nach innen schieben
            }
        }
        
    }
}


void UPauseScreen::createButtons(){
    createExitButton();
    createLoadoutScreenButton();
}

void UPauseScreen::createExitButton(){
    if(menu != nullptr && playerUiParent != nullptr){
        exitButton = NewObject<UTextButton>(this);

        if(exitButton){
            exitButton->init();
            exitButton->setText("exit"); //exit

            //set callback here
            exitButton->SetCallBack(
                FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::openGameScreen)
            );

            AddChildToMenu(*exitButton);
        }
       
    }
}


void UPauseScreen::createLoadoutScreenButton(){
    if(menu != nullptr && playerUiParent != nullptr){
        loadoutScreenButton = NewObject<UTextButton>(this);

        if(loadoutScreenButton){
            loadoutScreenButton->init();
            loadoutScreenButton->setText("Loadout"); //exit

            //set callback here
            loadoutScreenButton->SetCallBack(
                FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::openLoadoutScreen)
            );

            AddChildToMenu(*loadoutScreenButton);
        }
        
    }
}


///@brief adds a custom ui component to the menu.
void UPauseScreen::AddChildToMenu(UcustomUiComponentBase &item){
    if(menu != nullptr){
        UWidget *baseLayout = item.baseLayoutPointer();
        if(baseLayout != nullptr){
            menu->AddChildToVerticalBox(baseLayout);
        }
    }
}

