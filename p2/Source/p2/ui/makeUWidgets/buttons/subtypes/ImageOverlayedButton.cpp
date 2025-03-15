
#include "ImageOverlayedButton.h"
#include "p2/ui/makeUWidgets/OverlayBased/ImageOverlayed.h"
#include "Components/Button.h"


ImageOverlayedButton::ImageOverlayedButton(){

}
ImageOverlayedButton::ImageOverlayedButton(UPlayerUi &uiRef){
    saveParent(uiRef);
    createButton();
    createImageOverlayedBackground();

}

ImageOverlayedButton::~ImageOverlayedButton(){

}


void ImageOverlayedButton::createImageOverlayedBackground(){
    if(playerUiParent != nullptr){
        background = ImageOverlayed(*playerUiParent);

        if(button){
            UWidget *base = background.baseLayoutPointer();
            if(base != nullptr){
                //button->AddChild(base); //old
                scalebox->AddChild(base);
            }
        }
    }
}





//public api wrappers

void ImageOverlayedButton::setText(FString textIn){
    background.setText(textIn);
}

void ImageOverlayedButton::setImage(textureEnum type){
    background.setImage(type);
}

///@brief sets an icon for the image
///@param type - enum type of texture 
///@param scale value each component between 0.0 and 1.0 
void ImageOverlayedButton::setImage(textureEnum type, FVector2D scale){
    background.setImage(type, scale);
}