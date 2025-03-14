

#include "CanvasScreen.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"


///@brief will con struct the canvas screen and at itself to the canvas of the UPlayerUi
CanvasScreen::CanvasScreen(UPlayerUi &uiIn){
    saveParent(uiIn);
    createBaseCanvas();
}

void CanvasScreen::createBaseCanvas(){
    if(playerUiParent != nullptr){
        if(baseCanvas == nullptr){
            baseCanvas = NewObject<UCanvasPanel>(playerUiParent);
            playerUiParent->addToCanvas(baseCanvas);
            updateCanvasScale(playerUiParent->GetWorld());
        }
    }
}


void CanvasScreen::updateCanvasScale(UWorld *world){
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



void CanvasScreen::setVisible(bool visible){
    customUiComponentBase::setVisible(baseCanvas, visible);
}

UWidget *CanvasScreen::baseLayoutPointer(){
    return baseCanvas;
}