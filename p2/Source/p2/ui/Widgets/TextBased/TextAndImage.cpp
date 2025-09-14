


#include "AssetPlugin/gamestart/AssetLoader.h"
#include "GameCore/DebugHelper.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"

#include "TextAndImage.h"



void UTextAndImage::setImage(textureEnum type){
    setImage(type, FVector2D(0.5f, 0.5f));
}

///@brief sets an icon for the image
///@param type - enum type of texture 
///@param scale value each component between 0.0 and 1.0 
void UTextAndImage::setImage(textureEnum type, FVector2D scale){
    assetManager *pointer = assetManager::instance();
    if(pointer != nullptr){
        UTexture2D *loadedTexture = pointer->Find<textureEnum, UTexture2D>(type);//pointer->findTexture(type);
        setImage(loadedTexture, scale);
    }
}


void UTextAndImage::setImage(FString path){
    if(Image != nullptr){

        //DebugHelper::logMessage("ammuntionDebug try load texture");
        UTexture2D* loadedTexture = AssetLoader::loadAsset<UTexture2D>(path);
        setImage(loadedTexture);
    }
}
