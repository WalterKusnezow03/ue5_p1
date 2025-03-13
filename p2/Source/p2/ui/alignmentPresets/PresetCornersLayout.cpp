
#include "PresetCornersLayout.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"

PresetCornersLayout::PresetCornersLayout(){
    resetAllPointers();
}
PresetCornersLayout::PresetCornersLayout(UPlayerUi &parent){
    resetAllPointers();
    playerUiParent = &parent;
    createSubLayouts();
}
PresetCornersLayout::~PresetCornersLayout(){
    resetAllPointers();
}

void PresetCornersLayout::resetAllPointers(){
    playerUiParent = nullptr;
    topLeft = nullptr;
    topRight = nullptr;
    bottomLeft = nullptr;
    bottomRight = nullptr;
}



void PresetCornersLayout::createSubLayouts(){
    if(customUiComponentBase::correctInitialized()){


        UCanvasPanel *baseCanvas = playerUiParent->canvasPanelPointer();
        if(baseCanvas != nullptr){

            //create all sublayouts at corners

            topLeft = NewObject<UVerticalBox>(playerUiParent);
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(topLeft));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f));  // oben links
                CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f)); // An die Ecke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }


            topRight = NewObject<UVerticalBox>(playerUiParent);
            CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(topRight));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(1.0f, 0.0f));  // oben rechts
                CanvasSlot->SetAlignment(FVector2D(1.0f, 0.0f)); // An die Ecke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            bottomRight = NewObject<UVerticalBox>(playerUiParent);
            CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(bottomRight));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(1.0f, 1.0f));  // unten rechts
                CanvasSlot->SetAlignment(FVector2D(1.0f, 1.0f)); // An die uEcke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            bottomLeft = NewObject<UVerticalBox>(playerUiParent);
            CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(bottomLeft));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));  // unten rechts
                CanvasSlot->SetAlignment(FVector2D(0.0f, 1.0f)); // An die uEcke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }


        }
        
    }
}


//public api adding childs
void PresetCornersLayout::addChildTo(UVerticalBox *box, UWidget *any){
    if(any != nullptr && box != nullptr){
        if(correctInitialized()){
            box->AddChildToVerticalBox(any);
        }
    }
}

void PresetCornersLayout::addChildToTopLeft(UWidget *any){
    addChildTo(topLeft, any);
}
void PresetCornersLayout::addChildToTopRight(UWidget *any){
    addChildTo(topRight, any);
}
void PresetCornersLayout::addChildToBottomLeft(UWidget *any){
    addChildTo(bottomLeft, any);
}
void PresetCornersLayout::addChildToBottomRight(UWidget *any){
    addChildTo(bottomRight, any);
}
