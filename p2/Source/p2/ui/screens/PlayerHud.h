#pragma once

#include "Components/CanvasPanel.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "p2/ui/Widgets/TextBased/TextAndImage.h"
#include "p2/ui/Widgets/OverlayBased/ImageOverlayed.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/alignmentPresets/PresetCornersLayout.h"

#include "PlayerHud.generated.h"

class UPlayerUiBase;

UCLASS()
class P2_API UPlayerHud : public UCanvasScreen {
    GENERATED_BODY()
public:
    
    virtual void init(UPlayerUiBase &refin) override;

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