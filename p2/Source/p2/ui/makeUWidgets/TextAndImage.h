#pragma once

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"

#include "TextAndImage.generated.h"

class UPlayerUi;

/**
 * label with image like class in an horizontal layout
 */

UCLASS()
class P2_API UTextAndImage : public UcustomUiComponentBase{
    GENERATED_BODY()

public:
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override;

    void setText(FString textIn);

    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);
    void setImage(FString path);

private:
    bool isHorizontal = false;

    UHorizontalBox *baseHorizontalBox = nullptr;
    UVerticalBox *baseVerticalBox = nullptr;

    UTextBlock *TextBlock;
    UImage *Image;

    void createImage();
    void createText();
    bool correctInitialized();
};