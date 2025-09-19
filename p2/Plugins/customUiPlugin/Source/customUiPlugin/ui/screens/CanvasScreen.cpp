

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
    if(WAS_INIT_FLAG){
        return;
    }
    //playerUiParent = &refin;
    saveParent(refin);
    createBaseCanvas();
    CreateLayoutOnInit(); //empty on purpose
}

/// ---- Moving items ----
void UCanvasScreen::SetScreenPosition(UWidget *widget, FVector2D &ScreenPosition){
    if(widget){
        //APPLY OFFSET FROM TOP LEFT CORNER
        //get owning pointer of ui element and set to a position
        UCanvasPanelSlot *slot = Cast<UCanvasPanelSlot>(widget->Slot);
        if(slot != nullptr){
            slot->SetPosition(ScreenPosition); //kleiner test, nach innen schieben
        }
    }
}




/// ---- Adding items ----

/// @brief adds a child to the UCanvasPanel
void UCanvasScreen::AddChild(IBaseUiInterface *item){
    if(CanAddChild(item)){
        AddChild(item, FVector2D(0, 0));
    }
}

/// @brief adds a child to the UCanvasPanel with a ScreenPosition, listens for click automatically!
/// @param item 
/// @param offset 
void UCanvasScreen::AddChild(IBaseUiInterface *item, FVector2D ScreenPosition){
    if(item && baseCanvas){

        if(CanAddChild(item)){
            Super::AddChild(item); //click listening and widget track!
            
            UWidget *basePtr = item->baseLayoutPointer();
            AddChild(basePtr);
            // baseCanvas->AddChild(basePtr); //Add BEFORE SLOT* request!
            SetScreenPosition(basePtr, ScreenPosition);
        }
    }
}


void UCanvasScreen::AddChild(UWidget *widget){
    if(widget && baseCanvas){
        baseCanvas->AddChild(widget); //Add BEFORE SLOT* request!
    }
}






void UCanvasScreen::AddChild(
    IBaseUiInterface *item,
    FVector2D screenAnchor, //corner top left (0,0), bottom right (1,1)
    FVector2D alignment //gravity / pivot of item (0,0.5), make centered on y
){
    if(item && baseCanvas){

        if(CanAddChild(item)){
            Super::AddChild(item); //click listening
            

            UWidget *basePointer = item->baseLayoutPointer();
            if(basePointer){
                UCanvasPanelSlot *CanvasSlot = Cast<UCanvasPanelSlot>(baseCanvas->AddChild(basePointer));
                if(CanvasSlot != nullptr){
                    CanvasSlot->SetAnchors(FAnchors(screenAnchor.X, screenAnchor.Y));  //anchror from 2d(0,1) range
                    CanvasSlot->SetAlignment(alignment); //content alignment / pivot from 2d(0,1) range
                    CanvasSlot->SetAutoSize(true); //idk tbh
                }
            }
            
        }

    }
}


void UCanvasScreen::RemoveChild(IBaseUiInterface *item){
    if(item){
        Super::RemoveChild(item); //remove base interface from dispatch
        if(baseCanvas){
            UWidget *widget = item->baseLayoutPointer();
            if(widget){
                baseCanvas->RemoveChild(widget); //doesnt throw a exception if not contained.
            }
        }
    }
}

void UCanvasScreen::RemoveChild(UWidget *widget){
    if(widget){
        Super::RemoveChild(widget); //finds IBaseUiInterface from parent too and removes it from dispatch!
        if(baseCanvas){
            baseCanvas->RemoveChild(widget); // doesnt throw a exception if not contained.
        }
    }
}






#include "customUiPlugin/baseInterface/WidgetHelper.h"
void UCanvasScreen::SetVisible(bool visible){
    Super::SetVisible(visible);
    if(baseCanvas){
        WidgetHelper::SetVisible(baseCanvas, visible);
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