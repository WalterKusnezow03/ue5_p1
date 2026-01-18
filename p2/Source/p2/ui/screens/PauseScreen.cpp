
#include "p2/ui/PlayerUi.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "DebugPlugin/DebugHelper.h"
#include "AssetEnumCollection/assetEnums/textureEnum.h"
#include "PauseScreen.h"



void UPauseScreen::CreateLayoutOnInit(){
    //createBackgroundBlur();
    createBackgroundBlurAndDefaultColor();
    createMenu();
    createButtons();

    //debug();
    debugCreateGameLaunchButton();
}



// ---- DEBUG: MARK REMOVE ----
void UPauseScreen::debug(){
    if(menu){
        gridBox = NewObject<UGridBox>(this);
        gridBox->init(2, 2);
        menu->AddChild(gridBox);

        int count = 10;
        for (int i = 0; i < count; i++){
            UTextButton *someButton = NewWidgetInitialized<UTextButton>(this);
            someButton->SetText(FString::Printf(TEXT("Grid B(%d)"), i));
            gridBox->AddChild(someButton);
        }


        //remove 1 first for debug (should remove button(2,3))
        gridBox->RemoveRow(1);
    }
}

void UPauseScreen::debugCreateGameLaunchButton(){
   
    if(menu != nullptr && playerUiParent != nullptr){
        debugGameLaunchButton = NewObject<UTextButton>(this);

        if(debugGameLaunchButton){
            debugGameLaunchButton->init();
            debugGameLaunchButton->SetText("Game Launcher"); //exit

            //set callback here
            //cast to sub ui class because of function
            UPlayerUi *casted = Cast<UPlayerUi>(playerUiParent);
            if(casted){
                debugGameLaunchButton->SetCallBack(
                    FSimpleDelegate::CreateUObject(casted, &UPlayerUi::openGameLaunchScreen)
                );
            }

            

            menu->AddChild(debugGameLaunchButton); //automatically listened in click dispatcher because menu is listed
        }
    }



}





void UPauseScreen::createMenu(){
    if(menu == nullptr){
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
            exitButton->SetText("exit"); //exit

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
            loadoutScreenButton->SetText("Loadout"); //exit

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

