

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/PlayerUi.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"
#include "Components/CanvasPanelSlot.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/screens/loadout/buttons/WeaponPickButton.h"
#include "p2/ui/screens/loadout/buttons/AttachmentPickButton.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/alignmentPresets/PresetHalfSplitLayout.h"
#include "GameCore/DebugHelper.h"

#include "LoadoutScreen.h"



void ULoadoutScreen::init(UPlayerUi &ref){
    Super::init(ref);
    //createBackgroundBlur();
    createBackgroundBlurAndDefaultColor();
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

    /// ---- PICKABLE WEAPONS ----
    pickableWeaponsVertical = NewObject<UVerticalBox>(this);
    halfSplitBase->addChildToRightVertical(pickableWeaponsVertical, WEAPON_PICKER_IDENTIFIER); //ins 0te layout
    std::vector<weaponEnum> typesToHave = {
        weaponEnum::assaultRifle,
        weaponEnum::pistol
    };
    for (int i = 0; i < typesToHave.size(); i++)
    {
        UWeaponPickButton *weaponItem = NewObject<UWeaponPickButton>(this);
        if(weaponItem){
            weaponEnum currentweaponType = typesToHave[i];
            weaponItem->init();
            weaponItem->setType(currentweaponType);
            pickableWeaponsVertical->AddChildToVerticalBox(weaponItem->baseLayoutPointer());
            pickableWeaponsButtonVector.push_back(weaponItem);
        }
    }




    /// ---- PICKABLE SIGHTS ----
    pickableSightsVertical = NewObject<UVerticalBox>(this);
    halfSplitBase->addChildToRightVertical(pickableSightsVertical, SIGHT_PICKER_IDENTIFIER); //ins 1te layout
    std::vector<weaponAttachmentEnum> sightTypesToHave = {
        weaponAttachmentEnum::iron_sight,
        weaponAttachmentEnum::reddot
    };
    for (int i = 0; i < sightTypesToHave.size(); i++){
        UAttachmentPickButton *sightItem = NewObject<UAttachmentPickButton>(this);
        if(sightItem){
            weaponAttachmentEnum currentSightType = sightTypesToHave[i];
            sightItem->init();
            sightItem->setType(currentSightType);
            pickableSightsVertical->AddChildToVerticalBox(sightItem->baseLayoutPointer());
            pickableSightsButtonVector.push_back(sightItem);
        }
    }




    /// ---- PICKABLE MUZZLE ATTACHMENTS ----
    pickableMuzzlesVertical = NewObject<UVerticalBox>(this);
    halfSplitBase->addChildToRightVertical(pickableMuzzlesVertical, MUZZLE_PICKER_IDENTIFIER); //ins 1te layout
    std::vector<weaponAttachmentEnum> muzzleTypesToHave = {
        weaponAttachmentEnum::muzzle_flashSurpressor,
        weaponAttachmentEnum::muzzle_SoundSurpressor
    };
    for (int i = 0; i < muzzleTypesToHave.size(); i++){
        UAttachmentPickButton *muzzleItem = NewObject<UAttachmentPickButton>(this);
        if(muzzleItem){
            weaponAttachmentEnum currentType = muzzleTypesToHave[i];
            muzzleItem->init();
            muzzleItem->setType(currentType);
            pickableMuzzlesVertical->AddChildToVerticalBox(muzzleItem->baseLayoutPointer());
            pickableMuzzlesButtonVector.push_back(muzzleItem);
        }
    }


}





///@brief on click of any container on the left side, rebind
void ULoadoutScreen::rebindAllPickers(UWeaponContainer *currentWeaponContainerFocussed){
    if(currentWeaponContainerFocussed == nullptr){
        return;
    }

    //weapon pickers
    for (int i = 0; i < pickableWeaponsButtonVector.size(); i++){
        UWeaponPickButton *button = pickableWeaponsButtonVector[i];
        if(button){
            button->updateCallbackBind(currentWeaponContainerFocussed); //UPDATES CALLBACK INTERNALLY
        }
    }

    //sight pickers
    for (int i = 0; i < pickableSightsButtonVector.size(); i++){
        UAttachmentPickButton *button = pickableSightsButtonVector[i];
        if(button){
            button->updateCallbackBind(currentWeaponContainerFocussed); //UPDATES CALLBACK INTERNALLY
        }
    }

    

    //muzzle pickers
    for (int i = 0; i < pickableMuzzlesButtonVector.size(); i++){
        UAttachmentPickButton *button = pickableMuzzlesButtonVector[i];
        if(button){
            button->updateCallbackBind(currentWeaponContainerFocussed); //UPDATES CALLBACK INTERNALLY
        }
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
        FString sampleText = FString::Printf(TEXT("weapon %d"), index);
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

        //muzzle flash surpressor picker button
        weaponContainers[index]->setTextMuzzle("muzzle");
        weaponContainers[index]->SetCallBackMuzzle(
            FSimpleDelegate::CreateLambda([this, index]()
            {
                if(this){
                    this->openPickerCallBack(
                        this->MUZZLE_PICKER_IDENTIFIER,  
                        this->weaponContainers[index]
                    );
                } 
            })
        );



        halfSplitBase->addChildToLeftVertical(*weaponContainers[index]);
    }
}






/**
 * reload api from player ui
 */
void ULoadoutScreen::reloadPlayerLoadoutOnUiLeaveIfNeeded(){
    referenceManager::reloadPlayerLoadoutIfNeeded(loadoutinternal);
}