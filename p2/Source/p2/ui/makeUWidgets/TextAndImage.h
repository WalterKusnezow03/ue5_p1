#pragma once

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

class UPlayerUi;

class P2_API TextAndImage{

public:
    TextAndImage();
    TextAndImage(UPlayerUi &parent);
    ~TextAndImage();

    void setText(FString textIn);
    void setImage(FString path);

private:
    UPlayerUi *playerUiParent = nullptr;

    UHorizontalBox *baseHorizontalBox = nullptr;
    UTextBlock *TextBlock;
    UImage *Image;

    void createImage();
    void createText();
    bool correctInitiliazed();
};