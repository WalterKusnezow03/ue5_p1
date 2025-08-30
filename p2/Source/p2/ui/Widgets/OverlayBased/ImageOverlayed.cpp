
#include "ImageOverlayed.h"
#include "Components/Overlay.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "p2/ui/PlayerUi.h"



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
            UTexture2D *loadedTexture = pointer->Find<textureEnum, UTexture2D>(type);
            //pointer->findTexture(type);
            setImage(loadedTexture, scale);
        }
    }
    
}
