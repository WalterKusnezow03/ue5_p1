#pragma once

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "GameCore/util/timer.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"

#include "ImageOverlayedBase.generated.h"


/**
 * will display an image in background and a text at the top
 */

UCLASS()
class CUSTOMUIPLUGIN_API UImageOverlayedBase : public UcustomUiComponentBase{

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override {
        return baseOverlay;
    }

    void setText(FString message);
    void setTextTimed(FString message, float time);
    void setImage(UTexture2D *ptr);
    void setImage(UTexture2D *ptr, FVector2D scale);

    void showImage(bool hide);

    virtual void Tick(float DeltaTime) override;

protected:
    UOverlay *baseOverlay = nullptr;
    UTextBlock *TextBlock = nullptr;
    UImage *Image = nullptr;

    class timer textTimer;
    FString textOnEndTimer;

    void resetAllPointers();
    void createOverlay();
    void createImage();
    void createText();

    void TickTextTimer(float deltaTime);
};
