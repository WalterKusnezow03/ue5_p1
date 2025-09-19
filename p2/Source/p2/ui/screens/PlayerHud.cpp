
#include "PlayerHud.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/CanvasPanel.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"

#include "GameCore/DebugHelper.h"

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
                textureEnum::patroneIcon,
                FVector2D(0.25f, 1.0f) //scale quad texture to be long again.
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
                textureEnum::healthIcon,
                FVector2D(0.8f, 0.7f) //scale quad texture to be long again.
            );

            playerHudCornerLayout->addChildToBottomLeft(healthTextAndImage);
        }
    }
    
}

void UPlayerHud::createTopWarningElement(){
    if(playerHudCornerLayout){
        topWaringElement = NewWidgetInitialized<UImageOverlayed>(this);
        if(topWaringElement){
            
            playerHudCornerLayout->addChildToTopCenter(topWaringElement);


            updateTopWaringElement("top element");
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
        topWaringElement->SetText(message);
    }
}

///@brief updates the top text but will reset it after the time to live has exceeded
void UPlayerHud::updateTopWarningElementTimed(FString message, float timetolive){
    if(topWaringElement){
        topWaringElement->setTextTimed(message, timetolive);
    }
}


/// --- Tick ---
void UPlayerHud::Tick(float deltatime){
    Super::Tick(deltatime);
}

bool UPlayerHud::dispatchClick(){
    return false; //no click in hud.
}