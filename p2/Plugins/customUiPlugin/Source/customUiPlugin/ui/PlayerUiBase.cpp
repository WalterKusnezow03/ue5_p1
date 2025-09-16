// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUiBase.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "customUiPlugin/ui/TickUpdate/UiActor.h"
#include "GameCore/DebugHelper.h"


///@brief do not delete, only for ui components!
UCanvasPanel *UPlayerUiBase::canvasPanelPointer(){
    return baseCanvas;
}

/// @brief CALL ON FACTORY METHOD!
void UPlayerUiBase::init(UWorld *world){
    if(!world){
        return;
    }

    createTickActor(world);
    CreateCanvasPanel();

    //Add this ui to the viewport so it becomes visible!
    AddToViewport();
}


/// @brief creates actor for tick update!
/// @param world 
void UPlayerUiBase::createTickActor(UWorld *world){
    AUiActor *ptr = AUiActor::createInstance(world, this);
}

// ---- new instance from code ----
void UPlayerUiBase::CreateCanvasPanel()
{
    if (!WidgetTree)
        return;

    UCanvasPanel* NewCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(
        UCanvasPanel::StaticClass(), 
        TEXT("BaseCanvasInstance")
    );
    if (NewCanvas)
    {
        // Wenn es der Root-Widget sein soll:
        WidgetTree->RootWidget = NewCanvas;

        // Oder falls du es in ein bestehendes Panel hinzufügen willst:
        // UPanelWidget* Parent = ...
        // Parent->AddChild(NewCanvas);

        baseCanvas = NewCanvas;
    }
}




/// ----- add to own ------

///@brief will add any widget to the base canvas, if not nullptr!
void UPlayerUiBase::addToCanvas(UWidget *any){
    if(any != nullptr && baseCanvas != nullptr){
        baseCanvas->AddChild(any);
    }
}


void UPlayerUiBase::OpenScreen(UCanvasScreen *someScreen){
    if(someScreen){
        openedScreenStack.open(someScreen);
    }
}




void UPlayerUiBase::closeLatestScreen(){
    openedScreenStack.closeBack();
    if(openedScreenStack.isEmpty()){
        openGameScreen();
    }
}

void UPlayerUiBase::openGameScreen(){
    //designed to be overriden!
}




/// ----- MANUAL CLICK DISPATCHER ------

/// @brief manually registers clicks for UMG UI! Needed to be updated every frame!
void UPlayerUiBase::updateClickDispatch(){

    //click dispatch!
    TSet<FKey> PressedButtons = FSlateApplication::Get().GetPressedMouseButtons();
    if (PressedButtons.Contains(EKeys::LeftMouseButton)) {
        if(!bMouseLeftIsPressed){
            // Linksklick ist aktuell gedrückt und war es vorher nicht
            OnCursorClick();
            DebugHelper::showScreenMessage("UPlayerUiBase: SLATE CLICK REGISTERED", FColor::Red);    
        }
        bMouseLeftIsPressed = true;
    }else{
        bMouseLeftIsPressed = false;
    }

    //Hover dispatch
}

// new manual cursor dispatching clicks!
void UPlayerUiBase::OnCursorClick(){
    openedScreenStack.dispatchClick();
}


/// @brief ticks all children on the open screen
/// @param deltatime 
void UPlayerUiBase::Tick(float deltatime){
    openedScreenStack.Tick(deltatime);
}