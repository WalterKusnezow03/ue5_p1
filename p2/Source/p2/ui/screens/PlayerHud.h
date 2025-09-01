#pragma once

#include "Components/CanvasPanel.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "p2/ui/Widgets/TextBased/TextAndImage.h"
#include "p2/ui/Widgets/OverlayBased/ImageOverlayed.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/alignmentPresets/PresetCornersLayout.h"

#include "PlayerHud.generated.h"

struct FPlayerStatus;
class UPlayerUiBase;

UCLASS()
class P2_API UPlayerHud : public UCanvasScreen {
    GENERATED_BODY()
public:
    
    virtual void init(UPlayerUiBase &refin) override;

    
    void updateAmmunitionText(FString message);
    void updateHealthText(FString health);

    void updateTopWaringElement(FString message);
    void updateTopWarningElementTimed(FString message, float timetoLive);

    void Update(FPlayerStatus &playerStatusStruct);

private:

    //HUD SECTION
    UPROPERTY()
    UPresetCornersLayout *playerHudCornerLayout;

    UPROPERTY()
    UTextAndImage *ammunitionTextAndImage;

    UPROPERTY()
    UTextAndImage *healthTextAndImage;

    UPROPERTY()
    UImageOverlayed *topWaringElement;

    void createBasePlayerHud();
    void createAmmunitionHudElement();
    void createHealthHudElement();

    void createTopWarningElement();
};