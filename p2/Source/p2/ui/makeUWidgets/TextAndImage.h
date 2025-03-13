#pragma once

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"

class UPlayerUi;

class P2_API TextAndImage : public customUiComponentBase{

public:
    TextAndImage();
    TextAndImage(UPlayerUi &parent);
    ~TextAndImage();

    UWidget *layoutPointer();

    void setText(FString textIn);

    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);
    void setImage(FString path);

private:

    UHorizontalBox *baseHorizontalBox = nullptr;
    UTextBlock *TextBlock;
    UImage *Image;

    void createImage();
    void createText();
    virtual bool correctInitialized() override;
};