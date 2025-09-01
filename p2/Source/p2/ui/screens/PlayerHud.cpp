
#include "PlayerHud.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/CanvasPanel.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"

#include "GameCore/DebugHelper.h"

#include "p2/_world/PlayerStatManager/PlayerStatus.h"


void UPlayerHud::init(UPlayerUiBase &refin){
    //init base
    Super::init(refin);

    createBasePlayerHud();
    createAmmunitionHudElement();
    createHealthHudElement();
    createTopWarningElement();
}

/// ----- PLAYER HUD SECTION ----- START
void UPlayerHud::createBasePlayerHud(){
    
    playerHudCornerLayout = NewObject<UPresetCornersLayout>(this);
    if(playerHudCornerLayout){
        playerHudCornerLayout->init(this);
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
void UPlayerHud::Update(FPlayerStatus &playerStatusStruct){
    updateAmmunitionText(playerStatusStruct.AmmunitionString());
    updateHealthText(playerStatusStruct.HealthString());
}

void UPlayerHud::updateAmmunitionText(FString message){
    if(ammunitionTextAndImage){
        ammunitionTextAndImage->setText(message);
    }
}

void UPlayerHud::updateHealthText(FString message){
    if(healthTextAndImage){
        healthTextAndImage->setText(message);
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
