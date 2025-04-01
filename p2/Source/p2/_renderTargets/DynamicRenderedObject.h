#pragma once

#include "CoreMinimal.h"

/**
 * this object is designed to be owned by a parent object 
 */
class P2_API DynamicRenderedObject {

public:
    DynamicRenderedObject();
    DynamicRenderedObject(AActor *owner);
    ~DynamicRenderedObject();

    void updateTexture(UTexture2D *LoadedTexture);
    void updateTexture(FString path);

    UMaterialInstanceDynamic *getMaterial();

private:
    void init(AActor *owner);

    //ein render target kann von mehrerer umaterial instanzen / materials verwendet werden
    UCanvasRenderTarget2D *renderTarget = nullptr;

    //dynamic material created
    UMaterialInstanceDynamic *dynamicMaterial = nullptr;

    UTexture2D *loadTexture(FString path);


};