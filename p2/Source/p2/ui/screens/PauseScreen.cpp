
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "CanvasScreen.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"
#include "p2/DebugHelper.h"
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

void PauseScreen::createBackgroundBlur(){
    if(backgroundBlur == nullptr){
        backgroundBlur = NewObject<UBackgroundBlur>(playerUiParent);
        backgroundBlur->SetBlurStrength(30.0f); // Stärke des Weichzeichners
        backgroundBlur->SetBlurRadius(5);

        if(baseCanvas != nullptr){
            baseCanvas->AddChild(backgroundBlur);

            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(backgroundBlur));
            if(CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
                CanvasSlot->SetPosition(FVector2D(0, 0));
                //CanvasSlot->SetSize(FVector2D(1920, 1080)); // Adjust to viewport
            }
        }
    }
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
    debugCreateImageButton();
}

void PauseScreen::createExitButton(){
    if(menu != nullptr && playerUiParent != nullptr){
        exitButton = TextButton(*playerUiParent);
        exitButton.setText("exit"); //exit

        //set callback here
        exitButton.SetCallBack(
            FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::PauseKeyPressed)
        );

        UWidget *innerButton = exitButton.baseLayoutPointer();
        if(innerButton != nullptr){
            menu->AddChildToVerticalBox(innerButton);
        }
    }
}




void PauseScreen::debugCreateImageButton(){
    
    if(menu != nullptr && playerUiParent != nullptr){
        imageButton = ImageOverlayedButton(*playerUiParent);

        imageButton.setText("image button");
        imageButton.setImage(textureEnum::patroneIcon);

        UWidget *baseLayout = imageButton.baseLayoutPointer(); //always this function..

        menu->AddChildToVerticalBox(baseLayout);
    }
}