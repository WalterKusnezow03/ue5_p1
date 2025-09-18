
#include "ImageOverlayedButton.h"

/*
//IMPORTANT OVERRIDE

/// @brief creates a DERIVED CLASS instead of default image overlayed base! support ETextureLoading
void UImageOverlayedButton::createImageOverlayedBackground(){
    if(!background){
        background = NewObject<UImageOverlayed>(this);
        background->init();
    }
}*/




void UImageOverlayedButton::setImage(textureEnum type){
    setImage(type, FVector2D(0.5f, 0.5f));
}

///@brief sets an icon for the image
///@param type - enum type of texture 
///@param scale value each component between 0.0 and 1.0 
void UImageOverlayedButton::setImage(textureEnum type, FVector2D scale){
    if(background){
        assetManager *pointer = assetManager::instance();
        if(pointer != nullptr){
            UTexture2D *loadedTexture = pointer->Find<textureEnum, UTexture2D>(type);
            // pointer->findTexture(type);
            Super::setImage(loadedTexture, scale); //Super::
        }
    }
}