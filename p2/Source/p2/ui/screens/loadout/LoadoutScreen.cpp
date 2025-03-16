

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/PlayerUi.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"
#include "Components/CanvasPanelSlot.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/screens/loadout/buttons/WeaponPickButton.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/alignmentPresets/PresetHalfSplitLayout.h"
#include "p2/DebugHelper.h"

#include "LoadoutScreen.h"



void ULoadoutScreen::init(UPlayerUi &ref){
    Super::init(ref);
    createBackgroundBlur();
    createBaseLayout();
    createHeadline();


    createRightSideLoadoutMenuItems();
    createRigthListPickers();
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
                //FSimpleDelegate::CreateUObject(uclassInstance, &<classname>::<methodname>)
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






void ULoadoutScreen::createRightSideLoadoutMenuItems(){

    if(halfSplitBase){
        //slot 1 - 3

        //on click: open weapon picker (1-3) oder attachment pickers..
        //merken an welchem index das aktuelle item gespeichert ist

        weaponContainer1 = NewObject<UWeaponContainer>(this);
        if(weaponContainer1){
            weaponContainer1->init(); //immer init nicht vergessen

            //weapon button setup
            weaponContainer1->setTextWeapon("slot 1");
            weaponContainer1->SetCallBackWeapon(
                FSimpleDelegate::CreateLambda([this]()
                {
                    if(this){
                        //bei 0 zum beispiel weapon picker zeigen
                        this->openPickerCallBack(
                            this->WEAPON_PICKER_IDENTIFIER,  // Sollte eine gültige Konstante sein
                            this->weaponContainer1 // Direkt aus `this` holen
                        );
                    } 
                })
            );

            //sight button setup folgt hier..


            halfSplitBase->addChildToLeftVertical(*weaponContainer1);
        }






        //VERY IMPORTANT!
        //on create show first
        openPickerCallBack(WEAPON_PICKER_IDENTIFIER, weaponContainer1);

    }
    
}

///@brief CALLBACK FOR SLOT BUTTONS TO OPEN PICKERS
void ULoadoutScreen::openPickerCallBack(int index, UWeaponContainer *clickComingFrom){
    if(halfSplitBase != nullptr){
        if(clickComingFrom != nullptr){
            rebindAllPickers(clickComingFrom);
            halfSplitBase->showRightSideLayoutAtIndex(index);
        }
    }
}






//RIGHT SIDE PICKER BUTTONS

void ULoadoutScreen::createRigthListPickers(){
    

    //erste layout definieren und hier füllen
    pickableWeaponsVertical = NewObject<UVerticalBox>(this);
    halfSplitBase->addChildToRightVertical(pickableWeaponsVertical, WEAPON_PICKER_IDENTIFIER); //ins 0te layout

    UWeaponPickButton *weaponItem1 = NewObject<UWeaponPickButton>(this);
    if(weaponItem1){
        weaponItem1->init();
        weaponItem1->setText("pick > assault rifle");
        weaponItem1->setType(weaponEnum::assaultRifle);
        pickableWeaponsVertical->AddChildToVerticalBox(weaponItem1->baseLayoutPointer());
        pickableWeaponsButtonVector.push_back(weaponItem1);
    }
    


}





///@brief on click of any container on the left side, rebind
void ULoadoutScreen::rebindAllPickers(UWeaponContainer *currentWeaponContainerFocussed){
    //UWeaponContainer *currentWeaponContainerFocussed; //muss noch onclick gespeichert werden!

    for (int i = 0; i < pickableWeaponsButtonVector.size(); i++){
        UWeaponPickButton *button = pickableWeaponsButtonVector[i];
        if(button){
            setCallBackForSelection(button, currentWeaponContainerFocussed);
        }
    }
}


void ULoadoutScreen::setCallBackForSelection(
    UWeaponPickButton *buttonFromPickers, //another method for attachments later
    UWeaponContainer *bindedContainer //may be rebound trought this method!
){
    if(buttonFromPickers != nullptr && bindedContainer != nullptr){
        
        buttonFromPickers->SetCallBack(
            FSimpleDelegate::CreateLambda([bindedContainer, buttonFromPickers]()
            {
                if(bindedContainer != nullptr && buttonFromPickers != nullptr){
                    bindedContainer->updateWeaponType(buttonFromPickers->getType());
                    //UE_LOG(LogTemp, Warning, TEXT("DEBUGCALLBACK picker clicked"));
                    DebugHelper::logMessage("DEBUGCALLBACK picker clicked");
                } 
            })
        );
        DebugHelper::logMessage("DEBUGCALLBACK picker rebound callback");

    }
}