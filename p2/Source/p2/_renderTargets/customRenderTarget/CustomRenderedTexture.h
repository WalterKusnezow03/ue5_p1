#pragma once

#include "CoreMinimal.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "CustomRenderedTexture.generated.h"

UCLASS()
class P2_API UCustomRenderedTexture : public UObject{

    GENERATED_BODY()


public:
    UCustomRenderedTexture* Construct(AActor *owner);


    void Tick(float deltatime);

    UFUNCTION()
    virtual void CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height);

private:
    void setupRenderTarget();
    void setupMaterial();
    
    UCanvasRenderTarget2D *renderTarget = nullptr;
    UMaterialInterface* dynamicMaterial = nullptr;

    int resX = 4096;
    int resY = 4096;


};