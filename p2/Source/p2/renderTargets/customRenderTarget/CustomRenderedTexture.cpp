#include "CustomRenderedTexture.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "Engine/Canvas.h"



UCustomRenderedTexture* UCustomRenderedTexture::Construct(AActor *owner){
    if(owner != nullptr){
        UCustomRenderedTexture *ptr = NewObject<UCustomRenderedTexture>(owner);
        if(ptr){
            ptr->Init();
            return ptr;
        }
    }
    return nullptr;
}


UCustomRenderedTexture* UCustomRenderedTexture::Construct(AActor *owner, int resX, int resY){
    if(owner != nullptr){
        UCustomRenderedTexture *ptr = NewObject<UCustomRenderedTexture>(owner);
        if(ptr){
            ptr->Init(resX, resY);
            return ptr;
        }
    }
    return nullptr;
}


void UCustomRenderedTexture::Init(){
    Init(resX, resY);
}

void UCustomRenderedTexture::Init(int resXIn, int resYIn){
    setupRenderTarget(resX, resY);
    setupMaterial();
}


void UCustomRenderedTexture::setupRenderTarget(int resXIn, int resYIn){
    renderTarget = NewObject<UCanvasRenderTarget2D>(this);


    renderTarget->InitCustomFormat(resXIn, resYIn, PF_FloatRGBA, false);
            
    //Dynamische Methode binden
    renderTarget->OnCanvasRenderTargetUpdate.AddDynamic(
        this, &UCustomRenderedTexture::UCanvasUpdate
    );

    // Erstes Update anstoßen
    renderTarget->UpdateResource();

    //copy
    resX = resXIn;
    resY = resYIn;
}

void UCustomRenderedTexture::setupMaterial(){
    //material interface erstellen. Blueprint muss "RenderTargetTexture" als node haben!
    FString path = FString::Printf(
        TEXT("Blueprint'/Game/Prefabs/terrain/materials/texturedMaterialtransparent.texturedMaterialtransparent'")
    );

    UMaterialInterface *base = LoadObject<UMaterialInterface>(nullptr, *path);
    if(base != nullptr){
        UMaterialInstanceDynamic *dynamicMaterialInstance = UMaterialInstanceDynamic::Create(base, this);
        if(dynamicMaterialInstance){
            dynamicMaterialInstance->SetTextureParameterValue("rendertexture", renderTarget);
            //save pointer
            dynamicMaterial = dynamicMaterialInstance;
        }
    }
}

UMaterialInterface *UCustomRenderedTexture::getMaterial(){
    return dynamicMaterial;
}


FVector2D UCustomRenderedTexture::canvasScale(){
    return FVector2D(
        resX,
        resY
    );
}

FVector2D UCustomRenderedTexture::scalePercent(float percent){
    FVector2D scale = canvasScale();
    float scalar = percent * 0.01f;
    return scale * scalar;
}




/// @brief canvas to draw from player / ui component (for example minimap actor)
/// @param deltatime 
void UCustomRenderedTexture::Tick(float deltatime){
    if(renderTarget){
        renderTarget->UpdateResource(); // Triggert ReceiveUpdate() -> CanvasUpdate()
    }
}

/// @brief callback function on tick 
/// @param Canvas 
/// @param Width 
/// @param Height 
void UCustomRenderedTexture::UCanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height)
{
    CanvasUpdate(Canvas, Width, Height);
}

void UCustomRenderedTexture::CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height)
{
    if(Canvas == nullptr){
        return;
    }

    //override this function.
}


void UCustomRenderedTexture::drawImage(
    UCanvas *canvas,
    UTexture2D *texture,
    FVector &pos,
    FVector2D &scale,
    bool centerPivot
){
    FVector2D pos2D(pos.X, pos.Y);
    drawImage(canvas, texture, pos2D, scale, centerPivot);
}


void UCustomRenderedTexture::drawImage(
    UCanvas *canvas,
    UTexture2D *texture,
    FVector2D &pos,
    FVector2D &scale,
    bool centerPivot
){
    if(texture && canvas){

        FVector2D poscopy = pos;
        if(centerPivot){
            poscopy = centerPositionPivot(pos, scale);
        }

        /*
        DrawTile ( 
            UTexture* Tex,
            float X,
            float Y,
            float XL,
            float YL,
            float U,
            float V,
            float UL,
            float VL,
            EBlendMode BlendMode
        )
        */
        canvas->DrawTile ( 
            texture,
            poscopy.X,
            poscopy.Y,
            scale.X,
            scale.Y,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            EBlendMode::BLEND_Opaque//EBlendMode::BLEND_Translucent
        );
        //DebugHelper::showScreenMessage("scale marker", (float)scale.X);

    }
   
}






/*
----- rotated section -----
*/
void UCustomRenderedTexture::drawImage(
    UCanvas *canvas,
    UTexture2D* texture,
    MMatrix &transform,
    FVector2D &scale,
    bool centerPivot
){
    FRotator rotation = transform.extractRotator();
    rotation.Pitch = 0.0f;
    rotation.Roll = 0.0f;
    FVector pos3D = transform.getTranslation();
    FVector2D pos(pos3D.X, pos3D.Y);

    drawImage(
        canvas,
        texture,
        pos,
        scale,
        rotation,
        centerPivot
    );
}



void UCustomRenderedTexture::drawImage(
    UCanvas *canvas,
    UTexture2D* texture,
    FVector2D &pos,
    FVector2D &scale,
    FRotator &rotation,
    bool centerPivot
){
    if(canvas && texture){

        FVector2D poscopy = pos;
        if(centerPivot){
            poscopy = centerPositionPivot(pos, scale);
        }
        
        float rad = MMatrix::degToRadian(rotation.Yaw);
        canvas->K2_DrawTexture ( 
            texture,
            poscopy,
            scale,
            FVector2D(0,0),
            FVector2D(1,1),
            FLinearColor::White,
            EBlendMode::BLEND_Translucent,
            rotation.Yaw,
            FVector2D(0.5, 0.5) //pivot
        );
    }
}


/// @brief centers the position to draw the image from center pivot
/// @param pos 
/// @param scale 
/// @return 
FVector2D UCustomRenderedTexture::centerPositionPivot(
    FVector2D &pos, 
    FVector2D &scale
){
    FVector2D outpos = pos;
    outpos -= scale / 2.0f;
    return outpos;
}


UCanvasRenderTarget2D *UCustomRenderedTexture::GetRenderTarget(){
    return renderTarget;
}