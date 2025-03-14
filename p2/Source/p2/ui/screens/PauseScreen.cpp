
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "CanvasScreen.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/makeUWidgets/TextButton.h"
#include "p2/DebugHelper.h"
#include <functional>
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
    //createBackgroundBlur();

    createMenu();
    createButtons();
}

PauseScreen::~PauseScreen(){
    baseCanvas = nullptr;
    menu = nullptr;
    backgroundBlur = nullptr;
}

void PauseScreen::createBackgroundBlur(){
    if(backgroundBlur == nullptr){
        backgroundBlur = NewObject<UBackgroundBlur>(playerUiParent);
        backgroundBlur->SetBlurStrength(30.0f); // Stärke des Weichzeichners
        backgroundBlur->SetBlurRadius(5);

        if(baseCanvas != nullptr){
            baseCanvas->AddChild(backgroundBlur);
        }

        /*
        if(baseCanvas){
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(backgroundBlur));
            if(CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
                CanvasSlot->SetPosition(FVector2D(0, 0));
                //CanvasSlot->SetSize(FVector2D(1920, 1080)); // Adjust to viewport
            }
        }*/
    }
}
    


void PauseScreen::createMenu(){
    if(menu == nullptr && baseCanvas != nullptr){
        menu = NewObject<UVerticalBox>(playerUiParent);
        baseCanvas->AddChild(menu);
    }
}


void PauseScreen::createButtons(){
    createExitButton();
}

void PauseScreen::createExitButton(){
    if(menu != nullptr && playerUiParent != nullptr){
        exitButton = TextButton(*playerUiParent);
        exitButton.setText("exit");

        //set callback here
        exitButton.SetCallBack(
            FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::PauseKeyPressed)
        );

        UButton *innerButton = exitButton.buttonPoiner();
        if(innerButton != nullptr){
            menu->AddChildToVerticalBox(innerButton);
        }
    }
}