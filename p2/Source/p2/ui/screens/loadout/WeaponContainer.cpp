
#include "WeaponContainer.h"

#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "CoreMinimal.h"
#include "p2/weapon/enumUtil/WeaponAttachmentValidator.h"
#include "p2/weapon/enumUtil/WeaponEnumStringConverter.h"


void UWeaponContainer::init(UPlayerUiBase &ref){
    //ref is not needed
    init();
}

void UWeaponContainer::init(){
    createLayout();

    //default texts!
    setTextWeapon("weapon");
    setTextSight("sight");
    setTextMuzzle("muzzle");
}

///@brief inits the container, reference must not get invalid! - use this method to have the proper
///loadout updates!
/// @param parentLoadout MUST STAY VALID UNTIL THIS OBJECT DIES!
void UWeaponContainer::init(int index, LoadoutHelper &parentLoadout){
    ownIndexInLoadoutHelperParent = index;
    loadoutStorageParent = &parentLoadout;
    createLayout();
    initUiFromLoadoutParentAtOwnIndex();
}

// --- CUSTOM CLICK AND VISIBILTY ---
void UWeaponContainer::setVisible(bool flag){
    if(baseHorizontalBox){
        return baseHorizontalBox->setVisible(flag);
    }
}

bool UWeaponContainer::dispatchClick(){
    if(baseHorizontalBox){
        return baseHorizontalBox->dispatchClick();
    }
    return false;
}


// --- LAYOUT INIT ---

/// @brief creates Base Hbox and adds children for listening to click dispatcher
void UWeaponContainer::createLayout(){
    baseHorizontalBox = NewObject<UHbox>(this);
    if (baseHorizontalBox != nullptr){
        baseHorizontalBox->init();

        weaponPreviewImageButton = NewObject<UImageOverlayedButton>(this);
        if(weaponPreviewImageButton){
            weaponPreviewImageButton->init();
            weaponPreviewImageButton->setText("weapon image");

            //set on click later, weapon must know LoadoutScreen class to show weapon picker

            //addToHorizontalBox(weaponPreviewImageButton);
            baseHorizontalBox->AddChild(weaponPreviewImageButton);
        }

        attachmentSightImageButton = NewObject<UImageOverlayedButton>(this);
        if(attachmentSightImageButton){
            attachmentSightImageButton->init();
            attachmentSightImageButton->setText("sight image");

            //set on click later, weapon must know LoadoutScreen class to show attachment picker

            //addToHorizontalBox(attachmentSightImageButton);
            baseHorizontalBox->AddChild(attachmentSightImageButton);
        }

        attachmentMuzzleImageButton = NewObject<UImageOverlayedButton>(this);
        if(attachmentMuzzleImageButton){
            attachmentMuzzleImageButton->init();
            attachmentMuzzleImageButton->setText("muzzle image");

            //set on click later, weapon must know LoadoutScreen class to show attachment picker
            //addToHorizontalBox(attachmentMuzzleImageButton);
            baseHorizontalBox->AddChild(attachmentMuzzleImageButton);
        }
    }
}




/**
 * internal image and text updates
 */
void UWeaponContainer::updateAllNames(){
    updateMuzzleName();
    updateSightName();
    updateWeaponName();
    updateGripName();
}
void UWeaponContainer::updateWeaponName()
{
    weaponEnum type = setupinternal.getWeaponTypeToCreate();
    FString name = WeaponEnumStringConverter::toString(type);
    setTextWeapon(name);
}

void UWeaponContainer::updateSightName(){
    weaponAttachmentEnum type = setupinternal.getSightTypeToCreate();
    FString name = WeaponEnumStringConverter::toString(type);
    setTextSight(name);
}

void UWeaponContainer::updateMuzzleName(){
    weaponAttachmentEnum type = setupinternal.getMuzzleTypeToCreate();
    FString name = WeaponEnumStringConverter::toString(type);
    setTextMuzzle(name);
}

void UWeaponContainer::updateGripName(){
    weaponAttachmentEnum type = setupinternal.getGripTypeToCreate();
    FString name = WeaponEnumStringConverter::toString(type);
    
}


/**
 * ----- set text and images api -----
 */
void UWeaponContainer::setTextWeapon(FString s){
    if(weaponPreviewImageButton){
        weaponPreviewImageButton->setText(s);
    }
}

void UWeaponContainer::SetCallBackWeapon(FSimpleDelegate callbackIn){
    if(weaponPreviewImageButton){
        weaponPreviewImageButton->SetCallBack(callbackIn);
    }
}


void UWeaponContainer::setTextSight(FString s){
    if(attachmentSightImageButton){
        attachmentSightImageButton->setText(s);
    }
}

void UWeaponContainer::SetCallBackSight(FSimpleDelegate callbackIn){
    if(attachmentSightImageButton){
        attachmentSightImageButton->SetCallBack(callbackIn);
    }
}


void UWeaponContainer::setTextMuzzle(FString s){
    if(attachmentMuzzleImageButton){
        attachmentMuzzleImageButton->setText(s);
    }
}

void UWeaponContainer::SetCallBackMuzzle(FSimpleDelegate callbackIn){
    if(attachmentMuzzleImageButton){
        attachmentMuzzleImageButton->SetCallBack(callbackIn);
    }
}



/**
 * 
 * ----- CALLBACK API ------
 * 
 */
///@brief auto finds attachment types and sets it
void UWeaponContainer::updateAnyWeaponAttachment(weaponAttachmentEnum type){
    if(WeaponAttachmentValidator::isASightAttachment(type)){
        updateWeaponSight(type);
        return;
    }
    if(WeaponAttachmentValidator::isAMuzzleAttachment(type)){
        updateWeaponMuzzle(type);
        return;
    }
    if(WeaponAttachmentValidator::isAGripAttachment(type)){
        updateWeaponGrip(type);
        return;
    }
}




/**
 * 
 * ---- private callback api filter ----
 * 
 */
void UWeaponContainer::updateWeaponType(weaponEnum typeIn){
    setupinternal.setWeaponTypeToCreate(typeIn);
    updateWeaponName();
    updateLoadoutParent();
}

void UWeaponContainer::updateWeaponSight(weaponAttachmentEnum sightIn){
    setupinternal.setSightAttachment(sightIn);
    updateSightName();
    updateLoadoutParent();
}

void UWeaponContainer::updateWeaponMuzzle(weaponAttachmentEnum muzzlein){
    setupinternal.setMuzzleAttachment(muzzlein);
    updateMuzzleName();
    updateLoadoutParent();
}

void UWeaponContainer::updateWeaponGrip(weaponAttachmentEnum typein){
    setupinternal.setGripAttachment(typein);
    updateGripName();
    updateLoadoutParent();
}







void UWeaponContainer::updateFrom(weaponSetupHelper &other){
    setupinternal = other;
    updateAllNames();
    updateLoadoutParent();
}



/**
 * update own ui on init
 */

/// @brief loads loadout storage parent data at own index into ui elements
void UWeaponContainer::initUiFromLoadoutParentAtOwnIndex(){
    if(loadoutStorageParent){
        // real setup or fallback empty setup:
        weaponSetupHelper ownSetup = loadoutStorageParent->getIfValidIndex(ownIndexInLoadoutHelperParent);
        setupinternal = ownSetup;
        updateAllNames();
    }
}

/**
 * update parent when anything changed!
 */
weaponSetupHelper UWeaponContainer::setupHelperCopy(){
    return setupinternal;
}

/// @brief updates loadout parent with WeaponSetup preset held internally
void UWeaponContainer::updateLoadoutParent(){
    if(loadoutStorageParent != nullptr){

        //update loadout parent ptr with own weapon setup helper, override
        loadoutStorageParent->replace(ownIndexInLoadoutHelperParent, setupinternal);
    }
}