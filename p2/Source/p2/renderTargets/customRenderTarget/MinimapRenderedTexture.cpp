#include "MinimapRenderedTexture.h"
#include "Engine/Canvas.h"
#include "AssetPlugin/gamestart/assetManager.h"


UMinimapRenderedTexture* UMinimapRenderedTexture::ConstructMinimapTexture(AActor *owner){
    if(owner != nullptr){
        UMinimapRenderedTexture *ptr = NewObject<UMinimapRenderedTexture>(owner);
        if(ptr){
            ptr->Init();
            return ptr;
        }
    }
    return nullptr;
}


void UMinimapRenderedTexture::enableBackground(bool flag){
    drawbackgroundFlag = flag;
}

void UMinimapRenderedTexture::replaceMarkers(
    TArray<FVector> &positions, 
    textureEnum etexture
){
    markerMap[etexture] = positions;
}

void UMinimapRenderedTexture::replaceMarkers(
    TArray<MMatrix> &positions, 
    textureEnum etexture
){
    markerMapMatrix[etexture] = positions;
}


/// @brief callback function on tick 
/// @param Canvas 
/// @param Width 
/// @param Height 
void UMinimapRenderedTexture::CanvasUpdate(UCanvas* Canvas, int32 Width, int32 Height)
{
    if(Canvas == nullptr){
        return;
    }
    Super::CanvasUpdate(Canvas, Width, Height);

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
void UMinimapRenderedTexture::drawMarkers(
    UCanvas *canvas
){
    if(!canvas){
        return;
    }

    if(assetManager *manager = assetManager::instance()){
        FVector2D scaleForMarker = scalePercent(5.0f);
        
        bool centerPivot = true;

        //vector only based drawing
        for(auto &pair : markerMap){
            textureEnum type = pair.first;
            UTexture2D *texture = manager->Find<textureEnum, UTexture2D>(type);
            
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

        
        //matrix based daring (with rotation)
        for(auto &pair : markerMapMatrix){
            textureEnum type = pair.first;
            UTexture2D *texture = manager->Find<textureEnum, UTexture2D>(type);
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

