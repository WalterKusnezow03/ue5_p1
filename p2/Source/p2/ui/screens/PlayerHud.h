#pragma once

#include "Components/CanvasPanel.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"
#include "CanvasScreen.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"


class UPlayerUi;

class P2_API PlayerHud : public CanvasScreen {

public:

    PlayerHud();
    PlayerHud(UPlayerUi &playerui);
    ~PlayerHud();

   
    void updateAmmunitionText(int number);
    void updateAmmunitionText(FString message);
    void updateHealthText(int health);


private:

    //HUD SECTION
	PresetCornersLayout playerHudCornerLayout;
	TextAndImage ammunitionTextAndImage;
	TextAndImage healthTextAndImage;

    
    void createBasePlayerHud();
    void createAmmunitionHudElement();
    void createHealthHudElement();
};