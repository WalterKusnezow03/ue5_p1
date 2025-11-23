#pragma once

#include "CoreMinimal.h"
#include "Engine/CanvasRenderTarget2D.h"


#include "CustomRenderedTexture.generated.h"

class MMatrix;

/// @brief used for minimap.
UCLASS()
class P2_API UCustomRenderedTexture : public UObject{

    GENERATED_BODY()


public:
    static UCustomRenderedTexture* Construct(AActor *owner);
    static UCustomRenderedTexture* Construct(AActor *owner, int resX, int resY);


    void Tick(float deltatime);

    ///called by callback
    UFUNCTION()
    void UCanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height);

    //override this function
    virtual void CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height);

    UMaterialInterface *getMaterial();

    FVector2D canvasScale();

    ///for external use which should not be the case except camera feed
    ///rendering
    UCanvasRenderTarget2D *GetRenderTarget();

protected:
    void Init();
    void Init(int resXIn, int resYIn);

    void setupRenderTarget(int resXin, int resYin);
    void setupMaterial();
    

    UPROPERTY()
    UCanvasRenderTarget2D *renderTarget = nullptr;

    UPROPERTY()
    UMaterialInterface* dynamicMaterial = nullptr;

    int resX = 4096; //4096
    int resY = 4096; //4096


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