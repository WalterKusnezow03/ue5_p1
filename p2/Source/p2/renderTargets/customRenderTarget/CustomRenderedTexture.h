#pragma once

#include "CoreMinimal.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include <map>

#include "CustomRenderedTexture.generated.h"

class MMatrix;

UCLASS()
class P2_API UCustomRenderedTexture : public UObject{

    GENERATED_BODY()


public:
    static UCustomRenderedTexture* Construct(AActor *owner);


    void Tick(float deltatime);

    UFUNCTION()
    virtual void CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height);

    UMaterialInterface *getMaterial();

    FVector2D canvasScale();

    void replaceMarkers(TArray<FVector> &positions, textureEnum etexture);
    void replaceMarkers(TArray<MMatrix> &positions, textureEnum etexture);
 
    void enableBackground(bool flag);

private:
    void setupRenderTarget();
    void setupMaterial();
    
    bool drawbackgroundFlag = false;
    UCanvasRenderTarget2D *renderTarget = nullptr;
    UMaterialInterface* dynamicMaterial = nullptr;

    int resX = 4096; //4096
    int resY = 4096; //4096

    std::map<textureEnum, TArray<FVector>> markerMap;
    std::map<textureEnum, TArray<MMatrix>> markerMapMatrix;

    void drawMarkers(
        UCanvas *canvas
    );


    void drawImage(
        UCanvas *canvas,
        UTexture2D *texture,
        FVector &pos,
        FVector2D &scale,
        bool centerPivot
    );

    void drawImage(
        UCanvas *canvas,
        UTexture2D *texture,
        FVector2D &pos,
        FVector2D &scale,
        bool centerPivot
    );

    void drawImage(
        UCanvas *canvas,
        UTexture2D* texture,
        MMatrix &transform,
        FVector2D &scale,
        bool centerPivot
    );

    void drawImage(
        UCanvas *canvas,
        UTexture2D* texture,
        FVector2D &pos,
        FVector2D &scale,
        FRotator &rotation,
        bool centerPivot
    );

    FVector2D scalePercent(float percent);
    FVector2D centerPositionPivot(FVector2D &pos, FVector2D &scale);

};