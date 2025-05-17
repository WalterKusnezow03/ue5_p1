
#include "PlayerHud.h"
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"
#include "CanvasScreen.h"

#include "GameCore/DebugHelper.h"


void UPlayerHud::init(UPlayerUi &refin){
    saveParent(refin);

    createBaseCanvas(); //super.

    createBasePlayerHud();
    createAmmunitionHudElement();
    createHealthHudElement();
    createTopWarningElement();
}

/// ----- PLAYER HUD SECTION ----- START
void UPlayerHud::createBasePlayerHud(){
    if(baseCanvas){
        playerHudCornerLayout = NewObject<UPresetCornersLayout>(this);
        if(playerHudCornerLayout){
            playerHudCornerLayout->init(*baseCanvas);
        }
    }
}


void UPlayerHud::createAmmunitionHudElement(){
    if(playerHudCornerLayout){
        ammunitionTextAndImage = NewObject<UTextAndImage>(this);

        if(ammunitionTextAndImage){
            ammunitionTextAndImage->init();

            ammunitionTextAndImage->setImage(
                textureEnum::patroneIcon,
                FVector2D(0.25f, 1.0f) //scale quad texture to be long again.
            );

            UWidget *pointerOfTextImageLayout = ammunitionTextAndImage->baseLayoutPointer();
            if(pointerOfTextImageLayout != nullptr){
                
                playerHudCornerLayout->addChildToBottomRight(pointerOfTextImageLayout);
            }
        }
    }
    
}

void UPlayerHud::createHealthHudElement(){
    if(playerHudCornerLayout){

        healthTextAndImage = NewObject<UTextAndImage>(this);
        if(healthTextAndImage){
            healthTextAndImage->init();
            healthTextAndImage->setImage(
                textureEnum::healthIcon,
                FVector2D(0.8f, 0.7f) //scale quad texture to be long again.
            );

            UWidget *pointerOfTextImageLayout = healthTextAndImage->baseLayoutPointer();
            if(pointerOfTextImageLayout != nullptr){
                playerHudCornerLayout->addChildToBottomLeft(pointerOfTextImageLayout);
            }
        }
    }
    
}

void UPlayerHud::createTopWarningElement(){
    if(playerHudCornerLayout){
        topWaringElement = NewObject<UImageOverlayed>(this);
        if(topWaringElement){
            topWaringElement->init();

            UWidget *basePointerOfLayout = topWaringElement->baseLayoutPointer();
            if(basePointerOfLayout != nullptr){
                playerHudCornerLayout->addChildToTopCenter(basePointerOfLayout);
            }

            updateTopWaringElement("top element");
        }
    }
}

/// ----- PLAYER HUD SECTION ----- END

//PUBLIC API

void UPlayerHud::updateAmmunitionText(int number){
    FString message = FString::Printf(TEXT("%d"), number);
    updateAmmunitionText(message);
}

void UPlayerHud::updateAmmunitionText(FString message){
    if(ammunitionTextAndImage){
        ammunitionTextAndImage->setText(message);
    }
    
}

void UPlayerHud::updateHealthText(int health){
    if(healthTextAndImage){
        FString toText = FString::Printf(TEXT("%d"), health);
        healthTextAndImage->setText(toText);
    }
    
}


void UPlayerHud::updateTopWaringElement(FString message){
    if(topWaringElement){
        topWaringElement->setText(message);
    }
}

///@brief updates the top text but will reset it after the time to live has exceeded
void UPlayerHud::updateTopWarningElementTimed(FString message, float timetolive){
    if(topWaringElement){
        topWaringElement->setTextTimed(message, timetolive);
    }
}
