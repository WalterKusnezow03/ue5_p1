
#include "PlayerHud.h"
#include "p2/ui/PlayerUi.h"
#include "Components/CanvasPanel.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"
#include "CanvasScreen.h"

#include "p2/DebugHelper.h"


PlayerHud::PlayerHud(){
}

PlayerHud::PlayerHud(UPlayerUi &playerUiOwner){
    playerUiParent = &playerUiOwner;

    createBaseCanvas(); //super.

    createBasePlayerHud();
    createAmmunitionHudElement();
    createHealthHudElement();
}

PlayerHud::~PlayerHud(){

}


/// ----- PLAYER HUD SECTION ----- START
void PlayerHud::createBasePlayerHud(){
    if(baseCanvas != nullptr){
        playerHudCornerLayout = PresetCornersLayout(*playerUiParent, *baseCanvas);
    }
}


void PlayerHud::createAmmunitionHudElement(){

    ammunitionTextAndImage = TextAndImage(*playerUiParent);
    ammunitionTextAndImage.setImage(
        textureEnum::patroneIcon,
        FVector2D(0.25f, 1.0f) //scale quad texture to be long again.
    );

    UWidget *pointerOfTextImageLayout = ammunitionTextAndImage.baseLayoutPointer();
    if(pointerOfTextImageLayout != nullptr){
        playerHudCornerLayout.addChildToBottomRight(pointerOfTextImageLayout);
    }
}

void PlayerHud::createHealthHudElement(){
    healthTextAndImage = TextAndImage(*playerUiParent);
    healthTextAndImage.setImage(
        textureEnum::healthIcon,
        FVector2D(0.8f, 0.7f) //scale quad texture to be long again.
    );

    UWidget *pointerOfTextImageLayout = healthTextAndImage.baseLayoutPointer();
    if(pointerOfTextImageLayout != nullptr){
        playerHudCornerLayout.addChildToBottomLeft(pointerOfTextImageLayout);
    }
}


//PUBLIC API

void PlayerHud::updateAmmunitionText(int number){
    FString message = FString::Printf(TEXT("%d"), number);
    updateAmmunitionText(message);
}

void PlayerHud::updateAmmunitionText(FString message){
    ammunitionTextAndImage.setText(message);
}

void PlayerHud::updateHealthText(int health){
    FString toText = FString::Printf(TEXT("%d"), health);
    healthTextAndImage.setText(toText);
}

/// ----- PLAYER HUD SECTION ----- END