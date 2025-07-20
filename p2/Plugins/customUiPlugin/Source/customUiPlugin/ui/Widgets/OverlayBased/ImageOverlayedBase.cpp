
#include "ImageOverlayedBase.h"
#include "Components/Overlay.h"
#include "AssetPlugin/gamestart/assetManager.h"



void UImageOverlayedBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    resetAllPointers();
    createOverlay();
    createImage();
    createText();
}


void UImageOverlayedBase::resetAllPointers(){
    baseOverlay = nullptr;
    TextBlock = nullptr;
    Image = nullptr;
}


void UImageOverlayedBase::createOverlay(){
    baseOverlay = NewObject<UOverlay>(this);
}

void UImageOverlayedBase::createImage(){
    if(baseOverlay != nullptr){
        Image = NewObject<UImage>(this);
        if (Image){
            baseOverlay->AddChildToOverlay(Image);
            showImage(false);
        }
    }
}


void UImageOverlayedBase::createText(){
    if(baseOverlay != nullptr){
        TextBlock = NewObject<UTextBlock>(this);
        if (TextBlock){
            baseOverlay->AddChildToOverlay(TextBlock);
        }
    }
}





///@brief will set the text and kill the text timer if it was enabled
void UImageOverlayedBase::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
        Super::disableTick();
    }
}

void UImageOverlayedBase::setTextTimed(FString message, float time){
    if(TextBlock != nullptr){
        setText(message);
        textOnEndTimer = FString::Printf(TEXT(""));

        bool resetsItself = false;
        textTimer.Begin(time, resetsItself);
        //enable tick if not yet enabled
        Super::enableTick();
    }
}

//tick for text from base class
void UImageOverlayedBase::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    TickTextTimer(DeltaTime);
}

void UImageOverlayedBase::TickTextTimer(float deltaTime){
    if(!textTimer.timesUp()){
        textTimer.Tick(deltaTime);
        if(textTimer.timesUp()){
            setText(textOnEndTimer);
        }
    }
    
}


void UImageOverlayedBase::setImage(UTexture2D *ptr){
    setImage(ptr, FVector2D(0.5f, 0.5f));
}
void UImageOverlayedBase::setImage(UTexture2D *loadedTexture, FVector2D scale){
    if (loadedTexture != nullptr)
    {
        Image->SetBrushFromTexture(loadedTexture);
        Image->SetRenderScale(scale);
        showImage(true);
    }else{
        showImage(false);
    }
}



///@brief shows or hides the image
void UImageOverlayedBase::showImage(bool show){
    
    if(Image){
        ESlateVisibility newStatus = show ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
        //ESlateVisibility::Collapsed;
        Image->SetVisibility(newStatus);
    }

}