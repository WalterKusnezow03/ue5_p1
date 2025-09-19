
#include "PresetCornersLayout.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"


void UPresetCornersLayout::init(UCanvasScreen *canvasIn){
    if(WAS_INIT_FLAG){
        return;
    }
    
    if(canvasIn){
        parentPanel = canvasIn;
        createSubLayouts();
        WAS_INIT_FLAG = true;
    }
}


void UPresetCornersLayout::createSubLayouts(){
    if(true){

        if(parentPanel != nullptr){

            //create all sublayouts at corners

            topLeft = NewWidgetInitialized<UVbox>(parentPanel);
            addToParentPanel(topLeft, FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f));

            topRight = NewWidgetInitialized<UVbox>(parentPanel);
            addToParentPanel(topRight, FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f));

            bottomRight = NewWidgetInitialized<UVbox>(parentPanel);
            addToParentPanel(bottomRight, FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f));

            bottomLeft = NewWidgetInitialized<UVbox>(parentPanel);
            addToParentPanel(bottomLeft, FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f));

            topCenter = NewWidgetInitialized<UVbox>(parentPanel);
            addToParentPanel(topCenter, FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f));

        }
    }
}

/// @brief adds a item to the parent panel and inits the component!
/// @param item 
/// @param anchor 
/// @param alignment 
void UPresetCornersLayout::addToParentPanel(
    IBaseUiInterface *item, 
    FVector2D anchor, 
    FVector2D alignment
){
    if(item){
        UWidget *basePointer = item->baseLayoutPointer();
        if(basePointer){
            parentPanel->AddChild(item); //add to tick click dispatch
            UCanvasPanelSlot *CanvasSlot = Cast<UCanvasPanelSlot>(basePointer->Slot);
            if(CanvasSlot != nullptr){
                CanvasSlot->SetAnchors(FAnchors(anchor.X, anchor.Y));  //anchror from 2d(0,1) range
                CanvasSlot->SetAlignment(alignment); //content alignment from 2d(0,1) range
                CanvasSlot->SetAutoSize(true);
            }
        }
    }
}



///@brief will try to add a child to any vertical box, if both not nullptr
void UPresetCornersLayout::addChildTo(UVbox *box, IBaseUiInterface *any){
    if(any != nullptr && box != nullptr){
        box->AddChild(any);
    }
}

//public api add child WITH Click and visibilty listening
//public api adding childs
void UPresetCornersLayout::addChildToTopLeft(IBaseUiInterface *any){
    addChildTo(topLeft, any);
}
void UPresetCornersLayout::addChildToTopRight(IBaseUiInterface *any){
    addChildTo(topRight, any);
}
void UPresetCornersLayout::addChildToBottomLeft(IBaseUiInterface *any){
    addChildTo(bottomLeft, any);
}
void UPresetCornersLayout::addChildToBottomRight(IBaseUiInterface *any){
    addChildTo(bottomRight, any);
}

void UPresetCornersLayout::addChildToTopCenter(IBaseUiInterface *any){
    addChildTo(topCenter, any);
}

