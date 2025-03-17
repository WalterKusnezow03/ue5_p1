

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

    LoadoutContainersCreated = false;
    createLeftSideLoadoutMenuItems();
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






void ULoadoutScreen::createLeftSideLoadoutMenuItems(){

    if(halfSplitBase){
        //slot 1 - 3

        //on click: open weapon picker (1-3) oder attachment pickers..
        //merken an welchem index das aktuelle item gespeichert ist

        //SINGLE WEAPON
        /*
        weaponContainer1 = NewObject<UWeaponContainer>(this);
        if(weaponContainer1){
            weaponContainer1->init(); //immer init nicht vergessen

            //weapon button setup
            weaponContainer1->setTextWeapon("slot 1");
            weaponContainer1->SetCallBackWeapon(
                FSimpleDelegate::CreateLambda([this]()
                {
                    if(this){
                        this->openPickerCallBack(
                            this->WEAPON_PICKER_IDENTIFIER,  
                            this->weaponContainer1
                        );
                    } 
                })
            );

            //sight button setup folgt hier..
            weaponContainer1->setTextSight("sight 1"); //SIGHT_PICKER_IDENTIFIER
            weaponContainer1->SetCallBackSight(
                FSimpleDelegate::CreateLambda([this]()
                {
                    if(this){
                        this->openPickerCallBack(
                            this->SIGHT_PICKER_IDENTIFIER,  
                            this->weaponContainer1
                        );
                    } 
                })
            );


            halfSplitBase->addChildToLeftVertical(*weaponContainer1);
        }*/


        if(LoadoutContainersCreated){ //lock after creation
            return;
        }

        //WEAPON CONTAINER MUST NOT BE CHANGED AFTER INIT!
        weaponContainers.push_back(weaponContainer1);
        weaponContainers.push_back(weaponContainer2);
        for (int i = 0; i < weaponContainers.size(); i++)
        {
            createLoadoutWeaponContainerForValidIndex(i);
        }
        LoadoutContainersCreated = true; //lock after creation

        // VERY IMPORTANT!
        // on create show first
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





///@brief creates the selectable lists and adds them to the right side of the halfsplit preset
///by index, for example a list for the pickable weapons, sights, etc...
void ULoadoutScreen::createRigthListPickers(){
    

    //erste layout definieren und hier füllen
    pickableWeaponsVertical = NewObject<UVerticalBox>(this);
    halfSplitBase->addChildToRightVertical(pickableWeaponsVertical, WEAPON_PICKER_IDENTIFIER); //ins 0te layout

    UWeaponPickButton *weaponItem1 = NewObject<UWeaponPickButton>(this);
    if(weaponItem1){
        weaponItem1->init();
        weaponItem1->setType(weaponEnum::assaultRifle); //sets text automatically
        pickableWeaponsVertical->AddChildToVerticalBox(weaponItem1->baseLayoutPointer());
        pickableWeaponsButtonVector.push_back(weaponItem1);
    }
    


    UWeaponPickButton *weaponItem2 = NewObject<UWeaponPickButton>(this);
    if(weaponItem2){
        weaponItem2->init();
        weaponItem2->setType(weaponEnum::pistol); // sets text automatically
        pickableWeaponsVertical->AddChildToVerticalBox(weaponItem2->baseLayoutPointer());
        pickableWeaponsButtonVector.push_back(weaponItem2);
    }


}





///@brief on click of any container on the left side, rebind
void ULoadoutScreen::rebindAllPickers(UWeaponContainer *currentWeaponContainerFocussed){
    if(currentWeaponContainerFocussed == nullptr){
        return;
    }

    for (int i = 0; i < pickableWeaponsButtonVector.size(); i++){
        UWeaponPickButton *button = pickableWeaponsButtonVector[i];
        if(button){
            setCallBackForSelection(button, currentWeaponContainerFocussed);
        }
    }
}

///@brief sets the callback for a pick button from right side, to a container of the
/// left side (picked loadout item) --> call this on click of a pick container to
/// update the binding since the pickables are in n:m relation to the loadout items
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
                    DebugHelper::logMessage("DEBUGCALLBACK picker clicked");
                } 
            }));
        DebugHelper::logMessage("DEBUGCALLBACK picker rebound callback");

    }
}





///@brief setup a loadout item functionality from the weaponContainers by index, only setup once!
/// call once on construct only for every index
void ULoadoutScreen::createLoadoutWeaponContainerForValidIndex(
    int index
){
    if(LoadoutContainersCreated){
        return;
    }
    if(index < 0 || index >= weaponContainers.size()){
        return;
    }

    weaponContainers[index] = NewObject<UWeaponContainer>(this);
    if(weaponContainers[index]){
        //weaponContainers[index]->init(); //immer init nicht vergessen
        weaponContainers[index]->init(index, loadoutinternal);

        //weapon button setup
        FString sampleText = FString::Printf(TEXT("slot %d"), index);
        weaponContainers[index]->setTextWeapon(sampleText);
        weaponContainers[index]->SetCallBackWeapon(
            FSimpleDelegate::CreateLambda([this, index]()
            {
                if(this){
                    this->openPickerCallBack(
                        this->WEAPON_PICKER_IDENTIFIER,  
                        this->weaponContainers[index]
                    );
                } 
            })
        );

        //sight button setup folgt hier..
        weaponContainers[index]->setTextSight("sight"); //SIGHT_PICKER_IDENTIFIER
        weaponContainers[index]->SetCallBackSight(
            FSimpleDelegate::CreateLambda([this, index]()
            {
                if(this){
                    this->openPickerCallBack(
                        this->SIGHT_PICKER_IDENTIFIER,  
                        this->weaponContainers[index]
                    );
                } 
            })
        );


        halfSplitBase->addChildToLeftVertical(*weaponContainers[index]);
    }
}