
#include "PresetCornersLayout.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"


void UPresetCornersLayout::init(UCanvasPanel &canvasIn){
    if(WAS_INIT_FLAG){
        return;
    }

    parentPanel = &canvasIn;
    createSubLayouts();
}


void UPresetCornersLayout::createSubLayouts(){
    if(true){

        if(parentPanel != nullptr){

            //create all sublayouts at corners

            topLeft = NewObject<UVerticalBox>(this);
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(topLeft));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f));  // oben links
                CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f)); // An die Ecke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }


            topRight = NewObject<UVerticalBox>(this);
            CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(topRight));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(1.0f, 0.0f));  // oben rechts
                CanvasSlot->SetAlignment(FVector2D(1.0f, 0.0f)); // An die Ecke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            bottomRight = NewObject<UVerticalBox>(this);
            CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(bottomRight));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(1.0f, 1.0f));  // unten rechts
                CanvasSlot->SetAlignment(FVector2D(1.0f, 1.0f)); // An die uEcke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            bottomLeft = NewObject<UVerticalBox>(this);
            CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(bottomLeft));
            if (CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));  // unten rechts
                CanvasSlot->SetAlignment(FVector2D(0.0f, 1.0f)); // An die uEcke ausrichten
                CanvasSlot->SetAutoSize(true); // Automatische Größe anpassen
            }

            topCenter = NewObject<UVerticalBox>(this);
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
void UPresetCornersLayout::addChildTo(UVerticalBox *box, UWidget *any){
    if(any != nullptr && box != nullptr){
        box->AddChildToVerticalBox(any);
    }
}


//public api adding childs
void UPresetCornersLayout::addChildToTopLeft(UWidget *any){
    addChildTo(topLeft, any);
}
void UPresetCornersLayout::addChildToTopRight(UWidget *any){
    addChildTo(topRight, any);
}
void UPresetCornersLayout::addChildToBottomLeft(UWidget *any){
    addChildTo(bottomLeft, any);
}
void UPresetCornersLayout::addChildToBottomRight(UWidget *any){
    addChildTo(bottomRight, any);
}


void UPresetCornersLayout::addChildToTopCenter(UWidget *any){
    addChildTo(topCenter, any);
}
