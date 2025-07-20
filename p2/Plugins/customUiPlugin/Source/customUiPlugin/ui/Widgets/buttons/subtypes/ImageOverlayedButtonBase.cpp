
#include "ImageOverlayedButtonBase.h"
#include "customUiPlugin/ui/Widgets/OverlayBased/ImageOverlayedBase.h"
#include "Components/Button.h"



void UImageOverlayedButtonBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createImageOverlayedBackground();
    addBackgroundToBase();
}

/// @brief designed to be overriden by subclass if needed!
void UImageOverlayedButtonBase::createImageOverlayedBackground(){
    if(!background){
        background = NewObject<UImageOverlayedBase>(this);
        background->init();
    }
}


void UImageOverlayedButtonBase::addBackgroundToBase(){
    if (button && background)
    {
        UWidget *base = background->baseLayoutPointer();
        if(base != nullptr){
            //button->AddChild(base); //old
            scalebox->AddChild(base); //add to scalebox instead
        }

        makeTransparent(); //super
    }
}





//public api wrappers

void UImageOverlayedButtonBase::setText(FString textIn){
    if(background)
        background->setText(textIn);
}




void UImageOverlayedButtonBase::setImage(UTexture2D *ptr){
    setImage(ptr, FVector2D(0.5f, 0.5f));
}


void UImageOverlayedButtonBase::setImage(UTexture2D *loadedTexture, FVector2D scale){
    if(background)
        background->setImage(loadedTexture, scale);
}
