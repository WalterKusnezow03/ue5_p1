#include "CustomRenderedTexture.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "Engine/Canvas.h"



UCustomRenderedTexture* UCustomRenderedTexture::Construct(AActor *owner){
    if(owner != nullptr){
        UCustomRenderedTexture *ptr = NewObject<UCustomRenderedTexture>(owner);

        if(ptr){
            ptr->setupRenderTarget();
            ptr->setupMaterial();
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


void UCustomRenderedTexture::enableBackground(bool flag){
    drawbackgroundFlag = flag;
}



void UCustomRenderedTexture::replaceMarkers(
    TArray<FVector> &positions, 
    textureEnum etexture
){
    markerMap[etexture] = positions;
}

void UCustomRenderedTexture::replaceMarkers(
    TArray<MMatrix> &positions, 
    textureEnum etexture
){
    markerMapMatrix[etexture] = positions;
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
void UCustomRenderedTexture::CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height)
{
    if(Canvas == nullptr){
        return;
    }

    float lineThickness = Width / 10.0f;

    bool debugDrawMapBound = false;
    if(debugDrawMapBound){
        FLinearColor BackgroundColor = FLinearColor(1.0f,0.0f,0.0f,1.0f);
        Canvas->K2_DrawBox(FVector2D(0, 0), FVector2D(Width, Height), lineThickness, BackgroundColor);

        //debug corner
        BackgroundColor = FLinearColor(1.0f,1.0f,0.0f,1.0f);
        Canvas->K2_DrawBox(FVector2D(0, 0), FVector2D(100, 100), lineThickness, BackgroundColor);

        //draw center
        FVector2D center(Width / 2.0f, Height / 2.0f);
        FVector2D dir(50,50);
        Canvas->K2_DrawBox(center, dir, lineThickness / 4.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
    }


    if(drawbackgroundFlag){
        FVector2D pos(0,0);
        FVector2D size(Width, Height);
        float thickness = Width;
        FLinearColor color(1.0f,1.0f,1.0f, 0.5f);
        Canvas->K2_DrawBox(pos, size, thickness, color); //is opaque, not transculent (issue!)
    }
    
   

    //draw markers
    drawMarkers(Canvas);


    
}

/// @brief draws all markers on this canvas
/// @param canvas 
void UCustomRenderedTexture::drawMarkers(
    UCanvas *canvas
){
    if(!canvas){
        return;
    }

    if(assetManager *manager = assetManager::instance()){
        FVector2D scaleForMarker = scalePercent(5.0f);
        
        bool centerPivot = true;

        //vector only based
        for(auto &pair : markerMap){
            textureEnum type = pair.first;
            UTexture2D *texture = manager->findTexture(type);
            if(texture){
                TArray<FVector> &positions = pair.second;
                for(int i = 0; i < positions.Num(); i++){
                    drawImage(
                        canvas, 
                        texture,
                        positions[i],
                        scaleForMarker,
                        centerPivot
                    );
                }
            }
        }  

        
        //matrix based
        for(auto &pair : markerMapMatrix){
            textureEnum type = pair.first;
            UTexture2D *texture = manager->findTexture(type);
            if(texture){
                TArray<MMatrix> &positions = pair.second;
                for(int i = 0; i < positions.Num(); i++){
                    drawImage(
                        canvas, 
                        texture,
                        positions[i],
                        scaleForMarker,
                        centerPivot
                    );
                }
            }
        }   
    }

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