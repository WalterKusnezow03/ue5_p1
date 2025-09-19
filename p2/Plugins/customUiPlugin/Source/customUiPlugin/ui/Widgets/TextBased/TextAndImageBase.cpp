

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
    uniformScalingSetup = false;
}

bool UTextAndImageBase::dispatchClick(){
    if(!markedVisible()){
        return false;
    }
    return baseHorizontalBox != nullptr && baseHorizontalBox->dispatchClick();
}

void UTextAndImageBase::Tick(float deltatime){
    Super::Tick(deltatime);
    if (baseHorizontalBox)
    {
        //UiDebugHelper::logMessage("UTextAndImageBase tick"); is ticked.
        baseHorizontalBox->Tick(deltatime);
    }

    //set text and image same height, can only happen in tick
    //because slate widget building is not game thread :)
    if(TextBlock && Image){
        if(!uniformScalingSetup){
            //set image height same to text block
            FVector2D resText = TextBlock->GetResolution();
            if(resText.Y > 0.0f){
                Image->SetResolutionYUniform(resText.Y);
                uniformScalingSetup = true;
            }
        }
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