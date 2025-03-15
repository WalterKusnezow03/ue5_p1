

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/PlayerUi.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"
#include "Components/CanvasPanelSlot.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/alignmentPresets/PresetHalfSplitLayout.h"
#include "LoadoutScreen.h"



void ULoadoutScreen::init(UPlayerUi &ref){
    Super::init(ref);
    createBackgroundBlur();
    createBaseLayout();
    createHeadline();

}


void ULoadoutScreen::createBaseLayout(){
    if(baseCanvas != nullptr){
        halfSplitBase = NewObject<UPresetHalfSplitLayout>(this);
        halfSplitBase->init();

        if(halfSplitBase){
            halfSplitBase->init();

            UWidget *basePointer = halfSplitBase->baseLayoutPointer();
            if(basePointer != nullptr){
                baseCanvas->AddChild(basePointer);
                UCanvasPanelSlot *slotPointer = Cast<UCanvasPanelSlot>(basePointer->Slot);
                if(slotPointer != nullptr){
                    slotPointer->SetOffsets(FMargin(20.0f, 20.0f, 20.0f, 20.0f));
                }
            }
        }
    }
    
}

void ULoadoutScreen::createHeadline(){
    if(playerUiParent != nullptr && halfSplitBase != nullptr){

        //exit button
        exitButton = NewObject<UTextButton>(this);
        if(exitButton){
            exitButton->init();
            exitButton->setText("go back"); // exit
            exitButton->SetCallBack(
                FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::closeLatestScreen)
            );
            UWidget *baseOfExitButton = exitButton->baseLayoutPointer();
            if(baseOfExitButton){
                halfSplitBase->addChildToHeadLine(baseOfExitButton);
            }
        }
        


        //headline
        UTextBlock *newtext = NewObject<UTextBlock>(playerUiParent);
        if(newtext){
            newtext->SetText(FText::FromString("Loadout"));
            halfSplitBase->addChildToHeadLine(newtext);
        }
    }
}

void ULoadoutScreen::createSideMenu(){

}


void ULoadoutScreen::debugAddSomeItems(){
    
}
