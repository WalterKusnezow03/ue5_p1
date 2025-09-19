

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
    if(baseHorizontalBox){
        return baseHorizontalBox->baseLayoutPointer();
    }
    return nullptr;
}

void UTextAndImageBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    baseHorizontalBox = nullptr;
    TextBlock = nullptr;
    Image = nullptr;

    baseHorizontalBox = NewWidgetInitialized<UHbox>(this);

    createText();
    createImage();
    SetText("new text box image");

}

bool UTextAndImageBase::dispatchClick(){
    return baseHorizontalBox != nullptr && baseHorizontalBox->dispatchClick();
}

void UTextAndImageBase::Tick(float deltatime){
    Super::Tick(deltatime);
    if (baseHorizontalBox)
    {
        UiDebugHelper::logMessage("UTextAndImageBase tick");
        baseHorizontalBox->Tick(deltatime);
    }
}



void UTextAndImageBase::createText(){
    if(TextBlock == nullptr && baseHorizontalBox){
        TextBlock = NewObject<UWidgetSlateText>(this);
        baseHorizontalBox->AddChild((IBaseUiInterface*)TextBlock);
    }
}

void UTextAndImageBase::createImage(){
    if(Image == nullptr && baseHorizontalBox){
        Image = NewObject<UWidgetImage>(this);
        Image->SetResolution(FVector2D(50, 50));//debug, doesnt work
        baseHorizontalBox->AddChild((IBaseUiInterface*)Image);
    }
}



void UTextAndImageBase::SetText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(textIn);
    }
}


/// @brief sets the image texture from utexture2D* if not nullptr!
/// @param loadedTexture 
void UTextAndImageBase::setImage(UTexture2D *loadedTexture){
    setImage(loadedTexture, FVector2D(50,50));
}

void UTextAndImageBase::setImage(UTexture2D *loadedTexture, FVector2D scale){
    if (loadedTexture != nullptr && Image)
    {
        Image->SetImage(loadedTexture, scale);
    }
}