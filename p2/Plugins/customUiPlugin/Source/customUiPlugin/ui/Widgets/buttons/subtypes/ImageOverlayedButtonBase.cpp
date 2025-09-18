
#include "ImageOverlayedButtonBase.h"
#include "customUiPlugin/ui/Widgets/OverlayBased/ImageOverlayedBase.h"
#include "Components/Button.h"



void UImageOverlayedButtonBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createImageOverlayedBackground();
    //addBackgroundToBase();
}

/// @brief designed to be overriden by subclass if needed!
void UImageOverlayedButtonBase::createImageOverlayedBackground(){
    if(!background){
        background = NewObject<UWidgetImage>(this);
    }
}

void UImageOverlayedButtonBase::Tick(float deltatime){
    Super::Tick(deltatime);
    if(background){
        background->Tick(deltatime);
    }
}
bool UImageOverlayedButtonBase::dispatchClick(){
    if(background){
        return background->dispatchClick();
    }
    return false;
}

void UImageOverlayedButtonBase::setImage(UTexture2D *ptr){
    if(background){
        background->SetImage(ptr);
    }
}


void UImageOverlayedButtonBase::setImage(UTexture2D *loadedTexture, FVector2D scale){
    if(background){
        background->SetImage(loadedTexture, scale);
    }
}
