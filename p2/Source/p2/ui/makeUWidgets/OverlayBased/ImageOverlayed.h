#pragma once

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/util/timer.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"

#include "ImageOverlayed.generated.h"

class UPlayerUi;

/**
 * will display an image in background and a text at the top
 */

UCLASS()
class P2_API UImageOverlayed : public UcustomUiComponentBase{

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override {
        return baseOverlay;
    }

    void setText(FString message);
    void setTextTimed(FString message, float time);
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);

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
