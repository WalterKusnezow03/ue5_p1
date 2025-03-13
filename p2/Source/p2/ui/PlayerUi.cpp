// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "p2/gameStart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"

//instance maker with init call!
UPlayerUi* UPlayerUi::createNewInstance(UWorld *world){
    if(world != nullptr){
        assetManager *pointer = assetManager::instance();
        if(pointer != nullptr){
            UClass *blueprint = pointer->uiBp();

            if(blueprint != nullptr){

                UPlayerUi* newUiInstance = CreateWidget<UPlayerUi>(world, blueprint);
                if (newUiInstance != nullptr)
                {
                    newUiInstance->init();
                    return newUiInstance;
                }
            }
        }
    }
    return nullptr;
}


///@brief do not delete, only for ui components!
UCanvasPanel *UPlayerUi::canvasPanelPointer(){
    return baseCanvas;
}

//constructor like
void UPlayerUi::init(){
    if(isInited){
        return;
    }
    isInited = true;


    //the root widget must be a canvas panel in bp!
    findBaseCanvasFromBluePrint();

    // Jetzt kannst du mit dem CanvasPanel arbeiten
    createBasePlayerHud();
    createAmmunitionHudElement();
    createHealthHudElement();
}

void UPlayerUi::findBaseCanvasFromBluePrint(){
    UWidget* RootWidget = GetRootWidget();
    if (RootWidget)
    {
        UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(RootWidget);
        if (CanvasPanel)
        {
            baseCanvas = CanvasPanel;
        }
    }
}




/// ----- PLAYER HUD SECTION ----- START
void UPlayerUi::createBasePlayerHud(){
    playerHudCornerLayout = PresetCornersLayout(*this);
}


void UPlayerUi::createAmmunitionHudElement(){

    ammunitionTextAndImage = TextAndImage(*this);
    ammunitionTextAndImage.setImage(
        textureEnum::patroneIcon,
        FVector2D(0.25f, 1.0f) //scale quad texture to be long again.
    );

    UWidget *pointerOfTextImageLayout = ammunitionTextAndImage.layoutPointer();
    if(pointerOfTextImageLayout != nullptr){
        playerHudCornerLayout.addChildToBottomRight(pointerOfTextImageLayout);
    }
}

void UPlayerUi::createHealthHudElement(){
    healthTextAndImage = TextAndImage(*this);
    healthTextAndImage.setImage(
        textureEnum::healthIcon,
        FVector2D(0.8f, 0.7f) //scale quad texture to be long again.
    );

    UWidget *pointerOfTextImageLayout = healthTextAndImage.layoutPointer();
    if(pointerOfTextImageLayout != nullptr){
        playerHudCornerLayout.addChildToBottomLeft(pointerOfTextImageLayout);
    }
}




void UPlayerUi::updateAmmunitionText(int number){
    FString message = FString::Printf(TEXT("%d"), number);
    updateAmmunitionText(message);
}

void UPlayerUi::updateAmmunitionText(FString message){
    ammunitionTextAndImage.setText(message);
}

void UPlayerUi::updateHealthText(int health){
    FString toText = FString::Printf(TEXT("%d"), health);
    healthTextAndImage.setText(toText);
}

/// ----- PLAYER HUD SECTION ----- END