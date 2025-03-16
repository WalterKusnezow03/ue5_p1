
#include "ImageOverlayedButton.h"
#include "p2/ui/makeUWidgets/OverlayBased/ImageOverlayed.h"
#include "Components/Button.h"



void UImageOverlayedButton::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createImageOverlayedBackground();

}

void UImageOverlayedButton::createImageOverlayedBackground(){
    background = NewObject<UImageOverlayed>(this);
    background->init();
    if (button)
    {
        UWidget *base = background->baseLayoutPointer();
        if(base != nullptr){
            //button->AddChild(base); //old
            scalebox->AddChild(base);
        }
    }
}





//public api wrappers

void UImageOverlayedButton::setText(FString textIn){
    if(background)
        background->setText(textIn);
}

void UImageOverlayedButton::setImage(textureEnum type){
    if(background)
        background->setImage(type);
}

///@brief sets an icon for the image
///@param type - enum type of texture 
///@param scale value each component between 0.0 and 1.0 
void UImageOverlayedButton::setImage(textureEnum type, FVector2D scale){
    if(background)
        background->setImage(type, scale);
}