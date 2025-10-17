
#include "PlayerHud.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/CanvasPanel.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"

#include "DebugPlugin/DebugHelper.h"

#include "p2/_world/PlayerStatManager/PlayerStatus.h"



void UPlayerHud::CreateLayoutOnInit(){
    createBasePlayerHud();
    createAmmunitionHudElement();
    createHealthHudElement();
    createTopWarningElement();
}

/// ----- PLAYER HUD INIT SECTION ----- START
void UPlayerHud::createBasePlayerHud(){
    
    playerHudCornerLayout = NewObject<UPresetCornersLayout>(this);
    if(playerHudCornerLayout){
        playerHudCornerLayout->init(this);
    }
    
}


void UPlayerHud::createAmmunitionHudElement(){
    if(playerHudCornerLayout){
        ammunitionTextAndImage = NewWidgetInitialized<UTextAndImage>(this);
        if(ammunitionTextAndImage){
            ammunitionTextAndImage->setImage(
                textureEnum::patroneIcon
            );

            playerHudCornerLayout->addChildToBottomRight(ammunitionTextAndImage);
        }
    }
    
}

void UPlayerHud::createHealthHudElement(){
    if(playerHudCornerLayout){

        healthTextAndImage = NewWidgetInitialized<UTextAndImage>(this);
        if(healthTextAndImage){
            healthTextAndImage->setImage(
                textureEnum::healthIcon
            );

            playerHudCornerLayout->addChildToBottomLeft(healthTextAndImage);
        }
    }
    
}

void UPlayerHud::createTopWarningElement(){
    if(playerHudCornerLayout){
        topWaringElement = NewWidgetInitialized<UTextButton>(this);
        if(topWaringElement){
            playerHudCornerLayout->addChildToTopCenter(topWaringElement);

            updateTopWarningElementTimed("top element", 1.0f);
        }
    }
}

/// ----- PLAYER HUD INIT SECTION ----- END

//PUBLIC API
void UPlayerHud::Update(FPlayerStatus &playerStatusStruct){
    updateAmmunitionText(playerStatusStruct.AmmunitionString());
    updateHealthText(playerStatusStruct.HealthString());
}

void UPlayerHud::updateAmmunitionText(FString message){
    if(ammunitionTextAndImage){
        ammunitionTextAndImage->SetText(message);
    }
}

void UPlayerHud::updateHealthText(FString message){
    if(healthTextAndImage){
        healthTextAndImage->SetText(message);
    }
}


void UPlayerHud::updateTopWaringElement(FString message){
    if(topWaringElement){
        topWaringElement->SetVisible(true);
        topWaringElement->SetText(message);
    }
}

///@brief updates the top text but will reset it after the time to live has exceeded
void UPlayerHud::updateTopWarningElementTimed(FString message, float timetolive){
    updateTopWaringElement(message);
    topWarningTime = timetolive;
}

/// --- Tick ---
void UPlayerHud::Tick(float deltatime){
    Super::Tick(deltatime);

    DebugHelper::showScreenMessage("PlayerHud Tick");

    if(topWarningTime > 0.0f){
        topWarningTime -= deltatime;
        if(topWarningTime <= 0.0f){
            topWarningTime = 0.0f;
            topWaringElement->SetVisible(false);
        }
    }
}

bool UPlayerHud::dispatchClick(){
    return false; //no click in hud.
}