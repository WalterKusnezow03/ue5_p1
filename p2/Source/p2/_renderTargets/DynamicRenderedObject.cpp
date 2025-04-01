
#include "CoreMinimal.h"
#include "DynamicRenderedObject.h"

DynamicRenderedObject::DynamicRenderedObject(){
    dynamicMaterial = nullptr;
}
DynamicRenderedObject::DynamicRenderedObject(AActor *owner){
    dynamicMaterial = nullptr;
    init(owner);
}

DynamicRenderedObject::~DynamicRenderedObject(){

}


void DynamicRenderedObject::init(AActor *owner){
    if(owner != nullptr){
        renderTarget = NewObject<UCanvasRenderTarget2D>(owner);

        //das verstehe ich noch nicht.
        if(renderTarget){
            renderTarget->InitCustomFormat(1024, 1024, PF_FloatRGBA, false);
            renderTarget->UpdateResource(); //wozu


            //material interface erstellen. Blueprint muss "RenderTargetTexture" als node haben!
            FString path = FString::Printf(
                TEXT("Blueprint'/Game/Prefabs/terrain/materials/texturedMaterial.texturedMaterial'")
            );

            UMaterialInterface *base = LoadObject<UMaterialInterface>(nullptr, *path);
            if(base != nullptr){
                dynamicMaterial = UMaterialInstanceDynamic::Create(base, owner);
                if(dynamicMaterial){
                    dynamicMaterial->SetTextureParameterValue("rendertexture", renderTarget);
                }
            }
        }
        
    }
}



void DynamicRenderedObject::updateTexture(UTexture2D* LoadedTexture){
    if (LoadedTexture && dynamicMaterial) {
        dynamicMaterial->SetTextureParameterValue("rendertexture", LoadedTexture);
    }
}

void DynamicRenderedObject::updateTexture(FString path){
    UTexture2D *texture = loadTexture(path);
    if(texture){
        updateTexture(texture);
    }
}

///@brief format likestring : /Game/Textures/MyTexture.MyTexture
UTexture2D *DynamicRenderedObject::loadTexture(FString path){
    FString prefix = TEXT("Texture2D'");
    FString postfix = TEXT("'");
    FString combined = prefix + path + postfix;

    UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Textures/MyTexture.MyTexture'"));
    return LoadedTexture;
}


UMaterialInstanceDynamic *DynamicRenderedObject::getMaterial(){
    return dynamicMaterial;
}