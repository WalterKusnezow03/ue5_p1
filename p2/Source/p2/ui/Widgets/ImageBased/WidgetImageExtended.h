#pragma once
#include "customUiPlugin/slateDerived/ImageBased/ImageBase/WidgetImage.h"

#include "AssetPlugin/gamestart/assetManager.h"

#include "WidgetImageExtended.generated.h"

UCLASS()
class P2_API UWidgetImageExtended : public UWidgetImage {
    GENERATED_BODY()
public:
    using UWidgetImage::SetImage;

    void SetImage(textureEnum type){
        if(UTexture2D *found = findAsset(type)){
            Super::SetImage(found);
        }
    }

    void SetImage(textureEnum type, FVector2D fixedRes){
        if(UTexture2D *found = findAsset(type)){
            Super::SetImage(found, fixedRes);
        }
    }

private:
    UTexture2D *findAsset(textureEnum type){
        assetManager *pointer = assetManager::instance();
        if(pointer != nullptr){
            UTexture2D *loadedTexture = pointer->Find<textureEnum, UTexture2D>(type);
            return loadedTexture;
        }
        return nullptr;
    }
};