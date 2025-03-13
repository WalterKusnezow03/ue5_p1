

#include "UMG.h"
#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "p2/gameStart/AssetLoader.h"
#include "p2/DebugHelper.h"
#include "p2/ui/PlayerUi.h"
#include "p2/DebugHelper.h"
#include "p2/gameStart/assetEnums/textureEnum.h"

#include "TextAndImage.h"

TextAndImage::TextAndImage(){
    playerUiParent = nullptr;
    baseHorizontalBox = nullptr;
    TextBlock = nullptr;
    Image = nullptr;
}

TextAndImage::TextAndImage(UPlayerUi &parentIn){
    playerUiParent = nullptr;
    baseHorizontalBox = nullptr;
    TextBlock = nullptr;
    Image = nullptr;
    
    playerUiParent = &parentIn;

    baseHorizontalBox = NewObject<UHorizontalBox>(playerUiParent);
    if(baseHorizontalBox != nullptr){
        playerUiParent->addSelfToVerticalBox(baseHorizontalBox);
    }


    createText();
    setText("new text box image");
    createImage();



}

TextAndImage::~TextAndImage(){
    playerUiParent = nullptr;
    baseHorizontalBox = nullptr;
    TextBlock = nullptr;
    Image = nullptr;
}

bool TextAndImage::correctInitiliazed(){
    return playerUiParent != nullptr && baseHorizontalBox != nullptr;
}

void TextAndImage::createText(){
    if(TextBlock == nullptr && correctInitiliazed()){
        TextBlock = NewObject<UTextBlock>(playerUiParent);
        if (TextBlock){
            baseHorizontalBox->AddChildToHorizontalBox(TextBlock);
        }
    }
}

void TextAndImage::createImage(){
    if(Image == nullptr && correctInitiliazed()){
        Image = NewObject<UImage>(playerUiParent);
        if (Image){
            baseHorizontalBox->AddChildToHorizontalBox(Image);
        }
    }
}



void TextAndImage::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}


void TextAndImage::setImage(textureEnum type){
    assetManager *pointer = assetManager::instance();
    if(pointer != nullptr){
        UTexture2D *loadedTexture = pointer->findTexture(type);
        if (loadedTexture != nullptr)
        {
            Image->SetBrushFromTexture(loadedTexture);
            FVector2D scale(0.5f, 0.5f); // Example scale to 50% of the original size
            Image->SetRenderScale(scale);
        }
    }

}

void TextAndImage::setImage(FString path){
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