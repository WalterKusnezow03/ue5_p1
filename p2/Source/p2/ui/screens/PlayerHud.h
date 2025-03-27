#pragma once

#include "Components/CanvasPanel.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"
#include "p2/ui/makeUWidgets/OverlayBased/ImageOverlayed.h"
#include "CanvasScreen.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"

#include "PlayerHud.generated.h"

class UPlayerUi;

UCLASS()
class P2_API UPlayerHud : public UCanvasScreen {
    GENERATED_BODY()
public:
    
    virtual void init(UPlayerUi &refin) override;

    void updateAmmunitionText(int number);
    void updateAmmunitionText(FString message);
    void updateHealthText(int health);

    void updateTopWaringElement(FString message);
    void updateTopWarningElementTimed(FString message, float timetoLive);

private:

    //HUD SECTION
    UPresetCornersLayout *playerHudCornerLayout;
    UTextAndImage *ammunitionTextAndImage;
    UTextAndImage *healthTextAndImage;
    UImageOverlayed *topWaringElement;

    void createBasePlayerHud();
    void createAmmunitionHudElement();
    void createHealthHudElement();

    void createTopWarningElement();
};