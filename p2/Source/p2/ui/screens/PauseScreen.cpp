
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "CanvasScreen.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/Widgets/buttons/subtypes/TextButton.h"
#include "p2/ui/Widgets/buttons/subtypes/ImageOverlayedButton.h"
#include "GameCore/DebugHelper.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"
#include <functional>
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
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
        menu = NewObject<UVbox>(this);

        if(menu){
            menu->init(); //very important for custom click dispatcher

            //add menu to UCanvas in parent class, automatically tracked dispatcher / in click listening!
            //Super::
            AddChild(menu, FVector2D(100,100));
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

            menu->AddChild(exitButton); //automatically listened in click dispatcher because menu is listed
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

            menu->AddChild(loadoutScreenButton); //automatically listened in click dispatcher because menu is listed
        }
    }
}

