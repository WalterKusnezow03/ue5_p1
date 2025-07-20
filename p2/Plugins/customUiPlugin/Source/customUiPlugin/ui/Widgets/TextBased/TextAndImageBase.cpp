

#include "UMG.h"
#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameCore/DebugHelper.h"


#include "TextAndImageBase.h"

///@brief get the pointer of this layout and add it to an owning parent canvas for example!
UWidget *UTextAndImageBase::baseLayoutPointer(){
    return baseHorizontalBox;
}

void UTextAndImageBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    baseHorizontalBox = nullptr;
    TextBlock = nullptr;
    Image = nullptr;
   

    baseHorizontalBox = NewObject<UHorizontalBox>(this);

    createText();
    setText("new text box image");
    createImage();



}

bool UTextAndImageBase::correctInitialized(){
    return baseHorizontalBox != nullptr;
}

void UTextAndImageBase::createText(){
    if(TextBlock == nullptr && correctInitialized()){
        TextBlock = NewObject<UTextBlock>(this);
        if (TextBlock){
            baseHorizontalBox->AddChildToHorizontalBox(TextBlock);
        }
    }
}

void UTextAndImageBase::createImage(){
    if(Image == nullptr && correctInitialized()){
        Image = NewObject<UImage>(this);
        if (Image){
            baseHorizontalBox->AddChildToHorizontalBox(Image);
        }
    }
}



void UTextAndImageBase::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}


/// @brief sets the image texture from utexture2D* if not nullptr!
/// @param loadedTexture 
void UTextAndImageBase::setImage(UTexture2D *loadedTexture){

    setImage(loadedTexture, FVector2D(0.5f, 0.5f));
}

void UTextAndImageBase::setImage(UTexture2D *loadedTexture, FVector2D scale){
    if (loadedTexture != nullptr && Image)
    {
        Image->SetBrushFromTexture(loadedTexture);
        Image->SetRenderScale(scale);
    }
}