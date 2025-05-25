

#include "UMG.h"
#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AssetPlugin/gamestart/AssetLoader.h"
#include "GameCore/DebugHelper.h"
#include "p2/ui/PlayerUi.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"

#include "TextAndImage.h"

///@brief get the pointer of this layout and add it to an owning parent canvas for example!
UWidget *UTextAndImage::baseLayoutPointer(){
    return baseHorizontalBox;
}

void UTextAndImage::init(){
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

bool UTextAndImage::correctInitialized(){
    return baseHorizontalBox != nullptr;
}

void UTextAndImage::createText(){
    if(TextBlock == nullptr && correctInitialized()){
        TextBlock = NewObject<UTextBlock>(this);
        if (TextBlock){
            baseHorizontalBox->AddChildToHorizontalBox(TextBlock);
        }
    }
}

void UTextAndImage::createImage(){
    if(Image == nullptr && correctInitialized()){
        Image = NewObject<UImage>(this);
        if (Image){
            baseHorizontalBox->AddChildToHorizontalBox(Image);
        }
    }
}



void UTextAndImage::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}


void UTextAndImage::setImage(textureEnum type){
    setImage(type, FVector2D(0.5f, 0.5f));
}

///@brief sets an icon for the image
///@param type - enum type of texture 
///@param scale value each component between 0.0 and 1.0 
void UTextAndImage::setImage(textureEnum type, FVector2D scale){
    assetManager *pointer = assetManager::instance();
    if(pointer != nullptr){
        UTexture2D *loadedTexture = pointer->findTexture(type);
        if (loadedTexture != nullptr && Image != nullptr)
        {
            Image->SetBrushFromTexture(loadedTexture);
            Image->SetRenderScale(scale);
        }
    }
}




void UTextAndImage::setImage(FString path){
    if(Image != nullptr){

        DebugHelper::logMessage("ammuntionDebug try load texture");
        UTexture2D* loadedTexture = AssetLoader::loadAsset<UTexture2D>(path);
        if (loadedTexture != nullptr)
        {
            DebugHelper::logMessage("ammuntionDebug loaded texture");

            Image->SetBrushFromTexture(loadedTexture);
            FVector2D scale(0.5f, 0.5f); // Example scale to 50% of the original size
            Image->SetRenderScale(scale);
        }

    }
}