
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

            topLeft = NewObject<UVbox>(this);
            addToParentPanelAndInit(topLeft, FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f));

            topRight = NewObject<UVbox>(this);
            addToParentPanelAndInit(topRight, FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f));

            bottomRight = NewObject<UVbox>(this);
            addToParentPanelAndInit(bottomRight, FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f));

            bottomLeft = NewObject<UVbox>(this);
            addToParentPanelAndInit(bottomLeft, FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f));

            topCenter = NewObject<UVbox>(this);
            addToParentPanelAndInit(topCenter, FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f));

            elements.Add(topLeft);
            elements.Add(topRight);
            elements.Add(bottomRight);
            elements.Add(bottomLeft);
            elements.Add(topCenter);
        }
    }
}

/// @brief adds a item to the parent panel and inits the component!
/// @param item 
/// @param anchor 
/// @param alignment 
void UPresetCornersLayout::addToParentPanelAndInit(
    UcustomUiComponentBase *item, 
    FVector2D anchor, 
    FVector2D alignment
){
    if(item){
        item->init();
        UWidget *basePointer = item->baseLayoutPointer();
        if(basePointer){
            UCanvasPanelSlot *CanvasSlot = Cast<UCanvasPanelSlot>(parentPanel->AddChild(basePointer));
            if(CanvasSlot != nullptr){
                CanvasSlot->SetAnchors(FAnchors(anchor.X, anchor.Y));  //anchror from 2d(0,1) range
                CanvasSlot->SetAlignment(alignment); //content alignment from 2d(0,1) range
                CanvasSlot->SetAutoSize(true);
            }
        }
    }
}


//click dispatch
bool UPresetCornersLayout::dispatchClick(){
    bool found = false;
    for (int i = 0; i < elements.Num(); i++){
        UcustomUiComponentBase *current = elements[i];
        if(current){
            if(current->dispatchClick()){
                found = true;
            }
        }
    }
    return found;
}

/// @brief marks button as invisible: may be needed to not dispatch a click, base layout pointer is 
/// invisible too!
/// @param visible 
void UPresetCornersLayout::setVisible(bool visible) {
    Super::setVisible(visible);

    for (int i = 0; i < elements.Num(); i++){
        UcustomUiComponentBase *current = elements[i];
        if(current){
            current->setVisible(visible);
        }
    }
}

void UPresetCornersLayout::Tick(float deltatime){
    for (int i = 0; i < elements.Num(); i++){
        UcustomUiComponentBase *current = elements[i];
        if(current){
            current->Tick(deltatime);
        }
    }
}




///@brief will try to add a child to any vertical box, if both not nullptr
void UPresetCornersLayout::addChildTo(UVbox *box, UWidget *any){
    if(any != nullptr && box != nullptr){
        box->AddChild(any);
    }
}


///@brief will try to add a child to any vertical box, if both not nullptr
void UPresetCornersLayout::addChildTo(UVbox *box, UcustomUiComponentBase *any){
    if(any != nullptr && box != nullptr){
        box->AddChild(any);
    }
}

//public api add child WITH Click and visibilty listening
//public api adding childs
void UPresetCornersLayout::addChildToTopLeft(UcustomUiComponentBase *any){
    addChildTo(topLeft, any);
}
void UPresetCornersLayout::addChildToTopRight(UcustomUiComponentBase *any){
    addChildTo(topRight, any);
}
void UPresetCornersLayout::addChildToBottomLeft(UcustomUiComponentBase *any){
    addChildTo(bottomLeft, any);
}
void UPresetCornersLayout::addChildToBottomRight(UcustomUiComponentBase *any){
    addChildTo(bottomRight, any);
}

void UPresetCornersLayout::addChildToTopCenter(UcustomUiComponentBase *any){
    addChildTo(topCenter, any);
}




//public api adding childs UWIDGET - no click listen and visiblity listen
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
