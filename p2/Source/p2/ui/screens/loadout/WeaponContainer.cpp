

#include "Components/HorizontalBox.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/ui/PlayerUi.h"
#include "CoreMinimal.h"
#include "WeaponContainer.h"

void UWeaponContainer::init(UPlayerUi &ref){
    //ref is not needed
    init();
}

void UWeaponContainer::init(){
    createLayout();
}

///@brief inits the container, reference must not get invalid! - use this method to have the proper
///loadout updates!
void UWeaponContainer::init(int index, LoadoutHelper &parentLoadout){
    ownIndexInLoadoutHelperParent = index;
    loadoutParent = &parentLoadout;
    createLayout();
}


void UWeaponContainer::createLayout(){
    baseHorizontalBox = NewObject<UHorizontalBox>(this);
    if(baseHorizontalBox != nullptr){

        weaponPreviewImageButton = NewObject<UImageOverlayedButton>(this);
        if(weaponPreviewImageButton){
            weaponPreviewImageButton->init();
            weaponPreviewImageButton->setText("weapon image");

            //set on click later, weapon must know LoadoutScreen class to show weapon picker

            addToHorizontalBox(weaponPreviewImageButton);
        }

        attachmentSightImageButton = NewObject<UImageOverlayedButton>(this);
        if(attachmentSightImageButton){
            attachmentSightImageButton->init();
            attachmentSightImageButton->setText("sight image");

            //set on click later, weapon must know LoadoutScreen class to show attachment picker

            addToHorizontalBox(attachmentSightImageButton);
        }

        attachmentMuzzleImageButton = NewObject<UImageOverlayedButton>(this);
        if(attachmentMuzzleImageButton){
            attachmentMuzzleImageButton->init();
            attachmentMuzzleImageButton->setText("muzzle image");

            //set on click later, weapon must know LoadoutScreen class to show attachment picker
            addToHorizontalBox(attachmentMuzzleImageButton);
        }
    }
}

void UWeaponContainer::addToHorizontalBox(UcustomUiComponentBase *any){
    if(any != nullptr){
        UWidget *base = any->baseLayoutPointer();
        if(base != nullptr){
            baseHorizontalBox->AddChildToHorizontalBox(base);
        }
    }
}

/**
 * static methods
 */
FString UWeaponContainer::toString(weaponEnum type){
    FString name = FString::Printf(TEXT("<weapon name>"));
    if (type == weaponEnum::assaultRifle){
        name = FString::Printf(TEXT("assault rifle"));
    }
    if(type == weaponEnum::pistol){
        name = FString::Printf(TEXT("pistol"));
    }
    return name;
}

FString UWeaponContainer::toString(weaponAttachmentEnum type){
    FString name = FString::Printf(TEXT("<attachment name>"));
    
    if (type == weaponAttachmentEnum::iron_sight){
        name = FString::Printf(TEXT("iron sight"));
    }
    if (type == weaponAttachmentEnum::reddot){
        name = FString::Printf(TEXT("reddot sight"));
    }
    if (type == weaponAttachmentEnum::grip_vertical){
        name = FString::Printf(TEXT("grip vertical"));
    }
    
    return name;
} 



/**
 * internal image and text updates
 */
void UWeaponContainer::updateWeaponName(){
    weaponEnum type = setupinternal.getWeaponTypeToCreate();
    FString name = toString(type);
    setTextWeapon(name);
}

void UWeaponContainer::updateSightName(){
    weaponAttachmentEnum type = setupinternal.getSightTypeToCreate();
    FString name = toString(type);
    setTextSight(name);
}

void UWeaponContainer::updateMuzzleName(){
    weaponAttachmentEnum type = setupinternal.getMuzzleTypeToCreate();
    FString name = toString(type);
    setTextMuzzle(name);
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
    updateSightName();
    updateLoadoutParent();
}

void UWeaponContainer::updateFrom(weaponSetupHelper &other){
    setupinternal = other;
    updateWeaponName();
    updateSightName();
    updateLoadoutParent();
}

/**
 * update parent
 */
weaponSetupHelper UWeaponContainer::setupHelperCopy(){
    return setupinternal;
}

void UWeaponContainer::updateLoadoutParent(){
    if(loadoutParent != nullptr){

        //internal copy is made anyway
        loadoutParent->replace(ownIndexInLoadoutHelperParent, setupinternal);
    }
}