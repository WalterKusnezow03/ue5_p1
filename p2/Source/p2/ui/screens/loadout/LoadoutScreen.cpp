

#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "p2/ui/Widgets/buttons/subtypes/ImageOverlayedButton.h"
#include "Components/CanvasPanelSlot.h"
#include "AssetPlugin/gameStart/assetEnums/weaponEnum.h"
#include "p2/entityManager/referenceManager.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/screens/loadout/buttons/WeaponPickButton.h"
#include "p2/ui/screens/loadout/buttons/AttachmentPickButton.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "GameCore/DebugHelper.h"

#include "LoadoutScreen.h"



void ULoadoutScreen::init(UPlayerUiBase &ref){
    Super::init(ref);
    //createBackgroundBlur();
    createBackgroundBlurAndDefaultColor();
    createBaseLayout();
    createHeadline();

    LoadoutContainersCreated = false;
    createLeftSideLoadoutMenuItems();
    createRigthListPickers();
}


/// @brief creates base layout AND TRACKS COMPLETE LAYOUT IN CLICK LISTENING - AND ALL CHILDREN!
void ULoadoutScreen::createBaseLayout(){
    
    halfSplitBase = NewObject<UPresetHalfSplitLayout>(this);
    if(halfSplitBase){

        //TRACK IN CUTSOM CLICK DISPATCHING AND VISIBLITY
        //OWNS ALL ELEMENTS
        halfSplitBase->init(); //VERY IMPORTANT
        AddChild(halfSplitBase, FVector2D(20.0f, 20.0f));
    }

    /*
    //OLD
    if(baseCanvas != nullptr){
        halfSplitBase = NewObject<UPresetHalfSplitLayout>(this);

        if(halfSplitBase){

            //TRACK IN CUTSOM CLICK DISPATCHING AND VISIBLITY
            //OWNS ALL ELEMENTS
            halfSplitBase->init(); //VERY IMPORTANT
            AddClickListenedItem(halfSplitBase); 

            UWidget *basePointer = halfSplitBase->baseLayoutPointer();
            if(basePointer != nullptr){
                baseCanvas->AddChild(basePointer);
                UCanvasPanelSlot *slotPointer = Cast<UCanvasPanelSlot>(basePointer->Slot);
                if(slotPointer != nullptr){
                    slotPointer->SetOffsets(FMargin(20.0f, 20.0f, 20.0f, 20.0f));
                }
            }
        }
    }*/
    
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
            halfSplitBase->addChildToHeadLine(exitButton);


            //add to click listener!
            //AddClickListenedItem(exitButton);
        }
        


        //headline
        UTextBlock *newtext = NewObject<UTextBlock>(playerUiParent);
        if(newtext){
            newtext->SetText(FText::FromString("Loadout"));
            halfSplitBase->addChildToHeadLine(newtext);
        }
    }
}





/// @brief player load out on left Side of screen (Weapon containers by index)
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
        // on create show first weapon picker
        openPickerCallBack(WEAPON_PICKER_IDENTIFIER, 0);

    }
    
}

///@brief CALLBACK FOR SLOT BUTTONS TO OPEN PICKERS
/// pickers will update the correct container on the left side
void ULoadoutScreen::openPickerCallBack(int indexChooseType, int containerIndex){
    if(halfSplitBase != nullptr){
        if(containerIndex >= 0 && containerIndex < weaponContainers.size()){
            rebindAllPickers(containerIndex);
            halfSplitBase->showRightSideLayoutAtIndex(indexChooseType);
        }
    }
}





///@brief SELECTABLE STUFF FOR RIGHT OF HALF SPLIT BASE!
/// creates the selectable lists and adds them to the right side of the halfsplit preset
///by index, for example a list for the pickable weapons, sights, etc...
void ULoadoutScreen::createRigthListPickers(){
    

    //erste layout definieren und hier füllen

    /// ---- PICKABLE WEAPONS ----
    UVbox *pickableWeaponsVertical = NewObject<UVbox>(this); //OWNED BY HALFSPLIT BASE
    if(pickableWeaponsVertical){
        pickableWeaponsVertical->init(); //VERY IMPORTANT
        halfSplitBase->addChildToRightVertical(pickableWeaponsVertical, WEAPON_PICKER_IDENTIFIER); // ins 0te layout
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
                pickableWeaponsVertical->AddChild(weaponItem);
                pickableWeaponsButtonVector.push_back(weaponItem);

                //AddClickListenedItem(weaponItem); --> deprecated: abb vbox to listened items!
            }
        }
    }
    




    /// ---- PICKABLE SIGHTS ----
    UVbox *pickableSightsVertical = NewObject<UVbox>(this); //OWNED BY HALFSPLIT BASE
    if(pickableSightsVertical){
        pickableSightsVertical->init();
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
                pickableSightsVertical->AddChild(sightItem);
                pickableSightsButtonVector.push_back(sightItem);

                //AddClickListenedItem(sightItem); --> deprecated: abb vbox to listened items!
            }
        }
    }
    



    /// ---- PICKABLE MUZZLE ATTACHMENTS ----
    UVbox *pickableMuzzlesVertical = NewObject<UVbox>(this); //OWNED BY HALFSPLIT BASE
    if(pickableMuzzlesVertical){
        pickableMuzzlesVertical->init();
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
                pickableMuzzlesVertical->AddChild(muzzleItem);
                pickableMuzzlesButtonVector.push_back(muzzleItem);


                //AddClickListenedItem(muzzleItem); --> deprecated: abb vbox to listened items!
            }
        }
    }


}





///@brief on click of any container on the left side, rebind weapon container to be set up
void ULoadoutScreen::rebindAllPickers(int indexWeaponPickerFocussed){

    //find picker by index
    UWeaponContainer *currentWeaponContainerFocussed = nullptr;
    if(indexWeaponPickerFocussed >= 0 && indexWeaponPickerFocussed < weaponContainers.size()){
        currentWeaponContainerFocussed = weaponContainers[indexWeaponPickerFocussed];
    }

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
        weaponContainers[index]->SetCallBackWeapon(
            FSimpleDelegate::CreateLambda([this, index]()
            {
                if(this){
                    this->openPickerCallBack(
                        this->WEAPON_PICKER_IDENTIFIER,  
                        index //self weapon container
                    );
                } 
            })
        );

        //sight button setup folgt hier
        weaponContainers[index]->SetCallBackSight(
            FSimpleDelegate::CreateLambda([this, index]()
            {
                if(this){
                    this->openPickerCallBack(
                        this->SIGHT_PICKER_IDENTIFIER,  
                        index //self weapon container
                    );
                } 
            })
        );

        //muzzle flash surpressor picker button
        weaponContainers[index]->SetCallBackMuzzle(
            FSimpleDelegate::CreateLambda([this, index]()
            {
                if(this){
                    this->openPickerCallBack(
                        this->MUZZLE_PICKER_IDENTIFIER,  
                        index //self weapon container
                    );
                } 
            })
        );

        //add to left child always visible layout
        halfSplitBase->addChildToLeftVertical(weaponContainers[index]);



        //DEPRECATED!
        //LISTEN FOR INTERAL BUTTONS
        //AddClickListenedItems(weaponContainers[index]->internalButtons());
    }
}






/**
 * reload api from player ui
 */
void ULoadoutScreen::reloadPlayerLoadoutOnUiLeaveIfNeeded(){
    referenceManager::reloadPlayerLoadoutIfNeeded(loadoutinternal);
}