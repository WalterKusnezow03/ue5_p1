
#include "ImageOverlayed.h"
#include "Components/Overlay.h"
#include "p2/gamestart/assetManager.h"
#include "p2/ui/PlayerUi.h"


void UImageOverlayed::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    resetAllPointers();
    createOverlay();
    createImage();
    createText();
}


void UImageOverlayed::resetAllPointers(){
    baseOverlay = nullptr;
    TextBlock = nullptr;
    Image = nullptr;
}


void UImageOverlayed::createOverlay(){
    baseOverlay = NewObject<UOverlay>(this);
}

void UImageOverlayed::createImage(){
    if(baseOverlay != nullptr){
        Image = NewObject<UImage>(this);
        if (Image){
            baseOverlay->AddChildToOverlay(Image);
            showImage(false);
        }
    }
}


void UImageOverlayed::createText(){
    if(baseOverlay != nullptr){
        TextBlock = NewObject<UTextBlock>(this);
        if (TextBlock){
            baseOverlay->AddChildToOverlay(TextBlock);
        }
    }
}






void UImageOverlayed::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}


void UImageOverlayed::setImage(textureEnum type){
    setImage(type, FVector2D(0.5f, 0.5f));
}

///@brief sets an icon for the image
///@param type - enum type of texture 
///@param scale value each component between 0.0 and 1.0 
void UImageOverlayed::setImage(textureEnum type, FVector2D scale){
    if(Image){
        assetManager *pointer = assetManager::instance();
        if(pointer != nullptr){
            UTexture2D *loadedTexture = pointer->findTexture(type);
            if (loadedTexture != nullptr)
            {
                Image->SetBrushFromTexture(loadedTexture);
                Image->SetRenderScale(scale);
                showImage(true);
            }else{
                showImage(false);
            }
        }
    }
    
}



///@brief shows or hides the image
void UImageOverlayed::showImage(bool show){
    
    if(Image){
        ESlateVisibility newStatus = show ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
        //ESlateVisibility::Collapsed;
        Image->SetVisibility(newStatus);
    }

}