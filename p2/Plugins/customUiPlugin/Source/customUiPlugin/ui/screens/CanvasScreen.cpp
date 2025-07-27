

#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "Components/BackgroundBlur.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "customUiPlugin/ui/Widgets/buttons/colors/UiColors.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/PlayerUiBase.h"

#include "Components/CanvasPanelSlot.h"

///@brief sets up the canvas, the player ui parent is 
void UCanvasScreen::init(UPlayerUiBase &refin){
    playerUiParent = &refin;
    createBaseCanvas();

}

/// ---- Adding items ----

/// @brief adds a child to the UCanvasPanel
void UCanvasScreen::AddChild(UcustomUiComponentBase *item){
    AddChild(item, FVector2D(0, 0));
}

/// @brief adds a child to the UCanvasPanel with a offset
/// @param item 
/// @param offset 
void UCanvasScreen::AddChild(UcustomUiComponentBase *item, FVector2D offset){
    if(item && baseCanvas){
        UWidget *basePtr = item->baseLayoutPointer();
        if(basePtr){
            AddClickListenedItem(item); //Add to dispatcher!

            baseCanvas->AddChild(basePtr); //Add BEFORE SLOT* request!


            //APPLY OFFSET FROM TOP LEFT CORNER

            //get owning pointer of ui element and set to a position
            UCanvasPanelSlot *slot = Cast<UCanvasPanelSlot>(basePtr->Slot);
            if(slot != nullptr){
                slot->SetPosition(offset); //kleiner test, nach innen schieben
            }
        }

        
    }
}


// --- CLICK AND VISIBILITY DISPATCH ----

/// @brief dispatch click to all tracked items
/// @param mousePosition 
/// @return 
bool UCanvasScreen::dispatchClick(){
    for (int i = 0; i < listenForclickItems.Num(); i++){
        if(UcustomUiComponentBase *current = listenForclickItems[i]){
            if(current->dispatchClick()){
                return true;
            }
        }
    }

    return false;
}


void UCanvasScreen::setVisible(bool visible){
    for (int i = 0; i < listenForclickItems.Num(); i++){
        if(UcustomUiComponentBase *current = listenForclickItems[i]){
            current->setVisible(visible);
        }
    }


    //UcustomUiComponentBase::setVisible(baseCanvas, visible);
    UcustomUiComponentBase::setVisibleNoCollsion(baseCanvas, visible);
}


void UCanvasScreen::Tick(float deltatime){
    for (int i = 0; i < listenForclickItems.Num(); i++){
        if(UcustomUiComponentBase *current = listenForclickItems[i]){
            current->Tick(deltatime);
        }
    }
}








/// @brief ADD EACH BUTTON TO THIS DIPATCH POOL! add item to click dispatcher
/// @param ptr 
void UCanvasScreen::AddClickListenedItem(UcustomUiComponentBase *ptr){
    if(ptr){
        if(!listenForclickItems.Contains(ptr)){
            listenForclickItems.Add(ptr);
        }
    }
}

void UCanvasScreen::AddClickListenedItems(TArray<UcustomUiComponentBase *> array){
    for (int i = 0; i < array.Num(); i++){
        AddClickListenedItem(array[i]);
    }
}



/// @brief creates the canvas for this screen
void UCanvasScreen::createBaseCanvas(){
    if(playerUiParent != nullptr){
        if(baseCanvas == nullptr){
            baseCanvas = NewObject<UCanvasPanel>(this);
            playerUiParent->addToCanvas(baseCanvas);
            updateCanvasScale(playerUiParent->GetWorld());

            if(baseCanvas){
                //disbale collsion
                baseCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

            }
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



UWidget *UCanvasScreen::baseLayoutPointer(){
    return baseCanvas;
}






void UCanvasScreen::createBackgroundOverlay(){
    if(baseCanvas != nullptr){
        if(baseOverlay == nullptr){
            baseOverlay = NewObject<UOverlay>(this);

            if(baseOverlay){
                baseCanvas->AddChild(baseOverlay);

                UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(baseOverlay));
                if(CanvasSlot){
                    CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
                    CanvasSlot->SetPosition(FVector2D(0, 0));
                }
            }
            
        }
    }
    
}


void UCanvasScreen::createBackgroundBlur(){
    if(baseOverlay == nullptr){
        createBackgroundOverlay();
    }

    if(backgroundBlur == nullptr){
        backgroundBlur = NewObject<UBackgroundBlur>(this);
        backgroundBlur->SetBlurStrength(30.0f); // Stärke des Weichzeichners
        backgroundBlur->SetBlurRadius(5);

        if(baseOverlay != nullptr){
        //if(baseCanvas != nullptr){
            //new
            //baseOverlay->AddChild(backgroundBlur);
            UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(baseOverlay->AddChild(backgroundBlur));
            if (OverlaySlot) {
                OverlaySlot->SetHorizontalAlignment(HAlign_Fill); //fill setzten damit es nicht unsichtbar ist
                OverlaySlot->SetVerticalAlignment(VAlign_Fill);
            }

            //old
            /*
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(backgroundBlur));
            if(CanvasSlot){
                CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
                CanvasSlot->SetPosition(FVector2D(0, 0));
                //CanvasSlot->SetSize(FVector2D(1920, 1080)); // Adjust to viewport
            }*/
        }
    }
}





void UCanvasScreen::createColoredBackground(){
    if(baseOverlay == nullptr){
        createBackgroundOverlay();   
    }
    if(backgroundColor == nullptr && baseOverlay != nullptr){
        backgroundColor = NewObject<UBorder>(this);
        UOverlaySlot *overlaySlot = Cast<UOverlaySlot>(baseOverlay->AddChild(backgroundColor));
        if(overlaySlot){
            overlaySlot->SetHorizontalAlignment(HAlign_Fill); //fill setzten damit es nicht unsichtbar ist
            overlaySlot->SetVerticalAlignment(VAlign_Fill);
        }
    }
}

void UCanvasScreen::setBackgroundColor(FLinearColor color){
    if(backgroundColor == nullptr){
        createColoredBackground();
    }
    if(backgroundColor != nullptr){
        backgroundColor->SetBrushColor(color);  // Blue with 50% opacity
        //backgroundColor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

void UCanvasScreen::setDefaultBackgroundColor(){
    setBackgroundColor(UiColors::backgroundBlackTransculent);
}


void UCanvasScreen::createBackgroundBlurAndDefaultColor(){
    createBackgroundBlur();
    setDefaultBackgroundColor();
}