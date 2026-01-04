

#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

#include "Components/CanvasPanelSlot.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/entityManager/referenceManager.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "p2/weapon/enumUtil/WeaponEnumStringConverter.h"

#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/ui/screens/loadoutNew/SetupScreen/WeaponSetupHelperPayload.h"

#include "LoadoutScreen.h"



void ULoadoutScreen::init(UPlayerUiBase &ref){
    Super::init(ref);
    //createBackgroundBlur();
    createBackgroundBlurAndDefaultColor();
    
    
    //new
    FillOrLoadLoadout(); //must happens before filling layout / screen
    Createlayout();

    /*
    createHeadline();
    */
}

void ULoadoutScreen::Createlayout(){
    if(!baseVbox){
        baseVbox = NewWidgetInitialized<UVbox>(this);
        AddChild(baseVbox, FVector2D(0.1, 0.1), FVector2D(0, 0)); //item, anchor, gravity
    }

    //create exit button
    UHbox *heading = NewWidgetInitialized<UHbox>(this);
    baseVbox->AddChild(heading);

    //exit button
    UTextButton *button = NewWidgetInitialized<UTextButton>(this);
    button->SetText("Exit");
    heading->AddChild(button);
    button->SetCallBack(
        //FSimpleDelegate::CreateUObject(uclassInstance, &<classname>::<methodname>)
        FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUi::closeLatestScreen)
    );


    //create weapon list
    if(!weaponItemsInventory){
        weaponItemsInventory = NewWidgetInitialized<UVbox>(this);
        baseVbox->AddChild(weaponItemsInventory);


        //create buttons with special payload: weaponSetupHelper
        //do not modify array size when using buttons! Fixed size only! No resize!
        std::vector<weaponSetupHelper> &items = loadoutinternal.SetupHelpers();
        for (int i = 0; i < items.size(); i++){
            UTextButton *newButton = NewWidgetInitialized<UTextButton>(this);
            newButton->SetText("none");

            weaponSetupHelper *current = &items[i];
            UWeaponSetupHelperPayload *newPayLoad = NewObject<UWeaponSetupHelperPayload>(newButton);
            newPayLoad->Set(current);
            newButton->SetPayLoad(newPayLoad);

            weaponItemsInventory->AddChild(newButton);
        }
    }
    RefreshInventoryButtonNames();
}

void ULoadoutScreen::RefreshInventoryButtonNames(){
    if(weaponItemsInventory){
        /// @brief use only for very special cases where this data is needed!
        const TArray<IBaseUiInterface *> &buttons = weaponItemsInventory->AccessInternalItemsTmp();
        for (int i = 0; i < buttons.Num(); i++){
            IBaseUiInterface *current = buttons[i];
            if(current){
                UTextButton *casted = Cast<UTextButton>(current);
                if(casted){
                    RefreshButtonByOwnPayLoad(casted);
                }
            }
        }
    }
}

void ULoadoutScreen::RefreshButtonByOwnPayLoad(UTextButton *button){
    if(button){
        UPayLoadBase *payload = button->GetPayLoad();
        if(payload){
            UWeaponSetupHelperPayload *setupPayload = Cast<UWeaponSetupHelperPayload>(payload);
            if(setupPayload){
                weaponSetupHelper *helper = setupPayload->Get();
                if(helper){
                    weaponEnum typeWeapon = helper->getWeaponTypeToCreate();
                    FString displayName = WeaponEnumStringConverter::toString(typeWeapon);
                    button->SetText(displayName);
                }
            }
        }
    }
}





void ULoadoutScreen::FillOrLoadLoadout(){
    //might be filled from storage.
    for (int i = 0; i < 3; i++){
        weaponSetupHelper newSetup;
        loadoutinternal.push_back(newSetup); //is not allowed to be modified once buttons created,
        /// setup helpers attached by pointer!
    }
}






bool ULoadoutScreen::dispatchClick(const FVector2D &pos){
    if(!markedVisible()){
        return false;
    }
    bool flag = Super::dispatchClick(pos);
    bool here = false;

    //weapon buttons dont have a click delegate, instead hit test here
    if(weaponItemsInventory){
        const TArray<IBaseUiInterface *> &buttons = weaponItemsInventory->AccessInternalItemsTmp();
        for (int i = 0; i < buttons.Num(); i++){
            IBaseUiInterface *current = buttons[i];
            if(current){
                if(current->dispatchClick(pos)){
                    here = true;
                    OpenWeaponSetupScreen(current);
                }
            }
        }
    }
    


    return flag || here;
}


void ULoadoutScreen::OpenWeaponSetupScreen(IBaseUiInterface *buttonClicked){
    if(buttonClicked){
        //cast to component base for payload
        if(UcustomUiComponentBase *item = Cast<UcustomUiComponentBase>(buttonClicked)){
            UPayLoadBase *payload = item->GetPayLoad();
            if(payload){
                UWeaponSetupHelperPayload *castedSetupPayload = Cast<UWeaponSetupHelperPayload>(payload);
                if(castedSetupPayload){
                    weaponSetupHelper *internal = castedSetupPayload->Get();
                    OpenWeaponSetupScreen(internal);
                }
            }
        }
    }
}

void ULoadoutScreen::OpenWeaponSetupScreen(weaponSetupHelper *helper){
    if(playerUiParent && helper){
    
        if(!weaponSetupScreen){
            weaponSetupScreen = NewObject<UWeaponSetupScreen>(this);
            weaponSetupScreen->init(*playerUiParent);
        }

        if(weaponSetupScreen){
            
            //UPlayerUiBase::
            playerUiParent->OpenScreen(weaponSetupScreen);

            //refresh ui with weapon picker.
            weaponSetupScreen->UpdateScreen(*helper); //could be all done by index: safer!

            //weaponSetupHelper is not modifiable yet, but could be!
            //marked modfied in any case, easier handling. Weapons collected by objcet pool
            //anyway, doesnt make a difference (?)
            loadoutinternal.markModified();
        }
    } 
}


void ULoadoutScreen::OnReOpenScreen(){
    Super::OnReOpenScreen();
    RefreshInventoryButtonNames();
}



/**
 * reload api from player ui
 */
void ULoadoutScreen::reloadPlayerLoadoutOnUiLeaveIfNeeded(){
    referenceManager::reloadPlayerLoadoutIfNeeded(loadoutinternal);
}
