
#include "PresetCornersLayout.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"

PresetCornersLayout::PresetCornersLayout(){
    resetAllPointers();
}
PresetCornersLayout::PresetCornersLayout(UPlayerUi &parent, UCanvasPanel &canvasIn){
    resetAllPointers();
    parentPanel = &canvasIn;
    saveParent(parent);
    //playerUiParent = &parent;
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
    topCenter = nullptr;
    parentPanel = nullptr;
}

void PresetCornersLayout::createSubLayouts(){
    if(customUiComponentBase::correctInitialized()){

        if(parentPanel != nullptr){

            //create all sublayouts at corners

            topLeft = NewObject<UVerticalBox>(playerUiParent);
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(topLeft));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f));  // oben links
                CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f)); // An die Ecke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }


            topRight = NewObject<UVerticalBox>(playerUiParent);
            CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(topRight));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(1.0f, 0.0f));  // oben rechts
                CanvasSlot->SetAlignment(FVector2D(1.0f, 0.0f)); // An die Ecke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            bottomRight = NewObject<UVerticalBox>(playerUiParent);
            CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(bottomRight));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(1.0f, 1.0f));  // unten rechts
                CanvasSlot->SetAlignment(FVector2D(1.0f, 1.0f)); // An die uEcke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            bottomLeft = NewObject<UVerticalBox>(playerUiParent);
            CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(bottomLeft));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));  // unten rechts
                CanvasSlot->SetAlignment(FVector2D(0.0f, 1.0f)); // An die uEcke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            topCenter = NewObject<UVerticalBox>(playerUiParent);
            CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(topCenter));
            if(CanvasSlot != nullptr){
                CanvasSlot->SetAnchors(FAnchors(0.5f, 0.0f));  //mitte(?)
                CanvasSlot->SetAlignment(FVector2D(0.5f, 0.0f)); //content center aligned (?)
                CanvasSlot->SetAutoSize(true);
            }
        }
        
    }
}


///@brief will try to add a child to any vertical box, if both not nullptr
void PresetCornersLayout::addChildTo(UVerticalBox *box, UWidget *any){
    if(any != nullptr && box != nullptr){
        if(correctInitialized()){
            box->AddChildToVerticalBox(any);
        }
    }
}


//public api adding childs
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


void PresetCornersLayout::addChildToTopCenter(UWidget *any){
    addChildTo(topCenter, any);
}
