#pragma once

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/slateDerived/TextBased/Text/WidgetSlateText.h"
#include "customUiPlugin/slateDerived/ImageBased/ImageBase/WidgetImage.h"

#include "TextAndImageBase.generated.h"


/**
 * label with image like class in an horizontal layout
 */

UCLASS()
class CUSTOMUIPLUGIN_API UTextAndImageBase : public UcustomUiComponentBase{
    GENERATED_BODY()

public:
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override;

    void SetText(FString textIn);

    void setImage(UTexture2D *loadedTexture, FVector2D scale);
    void setImage(UTexture2D *loadedTexture);



    virtual bool dispatchClick() override;
    virtual void Tick(float deltatime) override;

protected:
    bool isHorizontal = false;

    UPROPERTY()
    UHbox *baseHorizontalBox = nullptr;
    
    UPROPERTY()
    UWidgetSlateText *TextBlock;

    UPROPERTY()
    UWidgetImage *Image;

    bool uniformScalingSetup = false;

    /*UPROPERTY()
    UTextBlock *TextBlock;

    UPROPERTY()
    UImage *Image;*/

    void createImage();
    void createText();
    
};