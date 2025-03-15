

#include "CanvasScreen.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"


void UCanvasScreen::init(UPlayerUi &refin){
    playerUiParent = &refin;
    createBaseCanvas();
}

void UCanvasScreen::createBaseCanvas(){
    if(playerUiParent != nullptr){
        if(baseCanvas == nullptr){
            baseCanvas = NewObject<UCanvasPanel>(this);
            playerUiParent->addToCanvas(baseCanvas);
            updateCanvasScale(playerUiParent->GetWorld());
        }
    }
}


void UCanvasScreen::updateCanvasScale(UWorld *world){
    if(world != nullptr && baseCanvas != nullptr){

        FVector2D ViewportSize;
        world->GetGameViewport()->GetViewportSize(ViewportSize);


        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->Slot);
        if (CanvasSlot)
        {
            CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));  
            CanvasSlot->SetPosition(FVector2D(0, 0));

            //DebugHelper::logMessage("canvasDebug size set");

            
        }
    }
}



void UCanvasScreen::setVisible(bool visible){
    UcustomUiComponentBase::setVisible(baseCanvas, visible);
}

UWidget *UCanvasScreen::baseLayoutPointer(){
    return baseCanvas;
}





void UCanvasScreen::createBackgroundBlur(){
    if(backgroundBlur == nullptr){
        backgroundBlur = NewObject<UBackgroundBlur>(this);
        backgroundBlur->SetBlurStrength(30.0f); // Stärke des Weichzeichners
        backgroundBlur->SetBlurRadius(5);

        if(baseCanvas != nullptr){
            baseCanvas->AddChild(backgroundBlur);

            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(backgroundBlur));
            if(CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
                CanvasSlot->SetPosition(FVector2D(0, 0));
                //CanvasSlot->SetSize(FVector2D(1920, 1080)); // Adjust to viewport
            }
        }
    }
}
    