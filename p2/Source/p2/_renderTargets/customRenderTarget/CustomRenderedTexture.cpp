#include "CustomRenderedTexture.h"




UCustomRenderedTexture* UCustomRenderedTexture::Construct(AActor *owner){
    if(owner != nullptr){
        UCustomRenderedTexture *ptr = NewObject<UCustomRenderedTexture>(owner);

        if(ptr){
            ptr->setupRenderTarget();
            ptr->setupMaterial();

            /** 
             *
             * ---- todo hier: static mesh aus actor holen: die texturieren! ---- 
             * 
            */


            return ptr;
        }
        
    }
    return nullptr;
}


void UCustomRenderedTexture::setupRenderTarget(){
    renderTarget = NewObject<UCanvasRenderTarget2D>(this);


    renderTarget->InitCustomFormat(resX, resY, PF_FloatRGBA, false);
            
    //Dynamische Methode binden
    renderTarget->OnCanvasRenderTargetUpdate.AddDynamic(
        this, &UCustomRenderedTexture::CanvasUpdate
    );

    // Erstes Update anstoßen
    renderTarget->UpdateResource();
}

void UCustomRenderedTexture::setupMaterial(){
    //material interface erstellen. Blueprint muss "RenderTargetTexture" als node haben!
    FString path = FString::Printf(
        TEXT("Blueprint'/Game/Prefabs/terrain/materials/texturedMaterial.texturedMaterial'")
    );

    UMaterialInterface *base = LoadObject<UMaterialInterface>(nullptr, *path);
    if(base != nullptr){
        UMaterialInstanceDynamic *dynamicMaterialInstance = UMaterialInstanceDynamic::Create(base, this);
        if(dynamicMaterialInstance){
            dynamicMaterialInstance->SetTextureParameterValue("rendertexture", renderTarget);
            dynamicMaterial = dynamicMaterialInstance;
        }
    }
}







void UCustomRenderedTexture::Tick(float deltatime){
    if(renderTarget){
        renderTarget->UpdateResource(); // Triggert ReceiveUpdate() -> CanvasUpdate()
    }
}


void UCustomRenderedTexture::CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height)
{
    // Beispiel: Weißen Hintergrund füllen
    FLinearColor BackgroundColor = FLinearColor::Black;
    Canvas->K2_DrawBox(FVector2D(0, 0), FVector2D(Width, Height), 0, BackgroundColor);

}