#pragma once

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"

class UPlayerUi;

/**
 * will display an image in background and a text at the top
 */
class P2_API ImageOverlayed : public customUiComponentBase{

public:
    ImageOverlayed();
    ImageOverlayed(UPlayerUi &parentIn);
    ~ImageOverlayed();

    virtual UWidget *baseLayoutPointer() override {
        return baseOverlay;
    }

    void setText(FString message);
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);

protected:
    UOverlay *baseOverlay = nullptr;
    UTextBlock *TextBlock = nullptr;
    UImage *Image = nullptr;

    void resetAllPointers();
    void createOverlay();
    void createImage();
    void createText();
};
