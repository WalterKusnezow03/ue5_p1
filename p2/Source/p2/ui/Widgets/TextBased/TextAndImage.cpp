


#include "AssetPlugin/gamestart/AssetLoader.h"
#include "DebugPlugin/DebugHelper.h"
#include "AssetEnumCollection/assetEnums/textureEnum.h"

#include "TextAndImage.h"



void UTextAndImage::setImage(textureEnum type){
    assetManager *pointer = assetManager::instance();
    if(pointer != nullptr){
        UTexture2D *loadedTexture = pointer->Find<textureEnum, UTexture2D>(type);//pointer->findTexture(type);
        setImage(loadedTexture);
    }
}



void UTextAndImage::setImage(FString path){
    if(Image != nullptr){

        //DebugHelper::logMessage("ammuntionDebug try load texture");
        UTexture2D* loadedTexture = AssetLoader::loadAsset<UTexture2D>(path);
        setImage(loadedTexture);
    }
}
