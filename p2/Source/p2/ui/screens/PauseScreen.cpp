
#include "p2/ui/PlayerUi.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "GameCore/DebugHelper.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "PauseScreen.h"



void UPauseScreen::init(UPlayerUiBase &playerUiParentref){
    Super::init(playerUiParentref);

    //createBackgroundBlur();
    createBackgroundBlurAndDefaultColor();
    createMenu();
    createButtons();

    debugCreateTextBox();
}

void UPauseScreen::debugCreateTextBox(){
    if(menu){
        debugTextBox = NewObject<UTextBoxBase>(this);
        if(debugTextBox){
            debugTextBox->init(); 
            menu->AddChild(debugTextBox); // add child to super
        }
    }
    
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



/// @brief creates the exit button to open the game screen (from play ui base class)
void UPauseScreen::createExitButton(){
    if(menu != nullptr && playerUiParent != nullptr){
        exitButton = NewObject<UTextButton>(this);

        if(exitButton){
            exitButton->init();
            exitButton->setText("exit"); //exit

            //set callback here
            exitButton->SetCallBack(
                FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUiBase::openGameScreen)
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
            //cast to sub ui class because of function
            UPlayerUi *casted = Cast<UPlayerUi>(playerUiParent);
            if(casted){
                loadoutScreenButton->SetCallBack(
                    FSimpleDelegate::CreateUObject(casted, &UPlayerUi::openLoadoutScreen)
                );
            }

            

            menu->AddChild(loadoutScreenButton); //automatically listened in click dispatcher because menu is listed
        }
    }
}

