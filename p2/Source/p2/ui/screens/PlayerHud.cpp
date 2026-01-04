
#include "PlayerHud.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/CanvasPanel.h"
#include "AssetEnumCollection/assetEnums/textureEnum.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"

#include "DebugPlugin/DebugHelper.h"

#include "p2/_world/PlayerStatManager/PlayerStatus.h"



void UPlayerHud::CreateLayoutOnInit(){
    createBasePlayerHud();
    
    createTopWarningElement();
}

/// ----- PLAYER HUD INIT SECTION ----- START
void UPlayerHud::createBasePlayerHud(){
    
    playerHudCornerLayout = NewObject<UPresetCornersLayout>(this);
    if(playerHudCornerLayout){
        playerHudCornerLayout->init(this);
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

    //DebugHelper::showScreenMessage("PlayerHud Tick");

    if(topWarningTime > 0.0f){
        topWarningTime -= deltatime;
        if(topWarningTime <= 0.0f){
            topWarningTime = 0.0f;
            topWaringElement->SetVisible(false);
        }
    }
}

bool UPlayerHud::dispatchClick(const FVector2D &pos){
    return false; //no click in hud.
}