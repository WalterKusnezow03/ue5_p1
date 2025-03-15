
#include "ImageOverlayed.h"
#include "Components/Overlay.h"
#include "p2/gamestart/assetManager.h"
#include "p2/ui/PlayerUi.h"


ImageOverlayed::ImageOverlayed(){
    resetAllPointers();
}
ImageOverlayed::ImageOverlayed(UPlayerUi &refin){
    resetAllPointers();
    saveParent(refin);
    createOverlay();
    createImage();
    createText();
}

ImageOverlayed::~ImageOverlayed(){
    resetAllPointers();
}

void ImageOverlayed::resetAllPointers(){
    baseOverlay = nullptr;
    TextBlock = nullptr;
    Image = nullptr;
}


void ImageOverlayed::createOverlay(){
    if(playerUiParent){
        baseOverlay = NewObject<UOverlay>(playerUiParent);
    }
}


void ImageOverlayed::createImage(){
    if(playerUiParent != nullptr && baseOverlay != nullptr){
        Image = NewObject<UImage>(playerUiParent);
        if (Image){
            baseOverlay->AddChildToOverlay(Image);
        }
    }
}


void ImageOverlayed::createText(){
    if(playerUiParent != nullptr && baseOverlay != nullptr){
        TextBlock = NewObject<UTextBlock>(playerUiParent);
        if (TextBlock){
            baseOverlay->AddChildToOverlay(TextBlock);
        }
    }
}






void ImageOverlayed::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}


void ImageOverlayed::setImage(textureEnum type){
    setImage(type, FVector2D(0.5f, 0.5f));
}

///@brief sets an icon for the image
///@param type - enum type of texture 
///@param scale value each component between 0.0 and 1.0 
void ImageOverlayed::setImage(textureEnum type, FVector2D scale){
    assetManager *pointer = assetManager::instance();
    if(pointer != nullptr){
        UTexture2D *loadedTexture = pointer->findTexture(type);
        if (loadedTexture != nullptr)
        {
            Image->SetBrushFromTexture(loadedTexture);
            Image->SetRenderScale(scale);
        }
    }
}