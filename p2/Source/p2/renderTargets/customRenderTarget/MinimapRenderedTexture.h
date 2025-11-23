#pragma once

#include "CoreMinimal.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include <map>
#include "CustomRenderedTexture.h"

#include "MinimapRenderedTexture.generated.h"

class MMatrix;

/// @brief used for minimap.
UCLASS()
class P2_API UMinimapRenderedTexture : public UCustomRenderedTexture{

    GENERATED_BODY()

public:
    static UMinimapRenderedTexture *ConstructMinimapTexture(AActor *owner);

    void replaceMarkers(TArray<FVector> &positions, textureEnum etexture);
    void replaceMarkers(TArray<MMatrix> &positions, textureEnum etexture);
 
    void enableBackground(bool flag);

    virtual void CanvasUpdate(UCanvas *Canvas, int32 Width, int32 Height) override;

private:

    bool drawbackgroundFlag = false;



    std::map<textureEnum, TArray<FVector>> markerMap;
    std::map<textureEnum, TArray<MMatrix>> markerMapMatrix;

    void drawMarkers(
        UCanvas *canvas
    );


};