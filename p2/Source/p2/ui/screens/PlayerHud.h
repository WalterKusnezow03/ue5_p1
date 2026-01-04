#pragma once

#include "Components/CanvasPanel.h"
#include "AssetEnumCollection/assetEnums/textureEnum.h"
#include "p2/ui/Widgets/TextBased/TextAndImage.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/alignmentPresets/PresetCornersLayout.h"

#include "customUiPlugin/ui/screens/WorldToScreenOverlays/canvas/WorldMarkerCanvas.h"

#include "PlayerHud.generated.h"

struct FPlayerStatus;
class UPlayerUiBase;

UCLASS()
class P2_API UPlayerHud : public UWorldMarkerCanvas {
//UCanvasScreen {
    GENERATED_BODY()
public:

    virtual bool UsesUserInput() override {
        return false;
    }
    
    //virtual void init(UPlayerUiBase &refin) override;
    virtual void CreateLayoutOnInit() override;

    virtual void Tick(float deltatime) override;
    virtual bool dispatchClick(const FVector2D &pos) override;


    void updateTopWaringElement(FString message);
    void updateTopWarningElementTimed(FString message, float timetoLive);


private:

    //HUD SECTION
    UPROPERTY()
    UPresetCornersLayout *playerHudCornerLayout;


    UPROPERTY()
    UTextButton *topWaringElement;

    float topWarningTime = 0.0f;

    void createBasePlayerHud();
    void createTopWarningElement();

};