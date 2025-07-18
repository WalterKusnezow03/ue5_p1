

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
    if(type == weaponAttachmentEnum::muzzle_flashSurpressor){
        name = FString::Printf(TEXT("flash surpressor"));
    }
    if(type == weaponAttachmentEnum::muzzle_SoundSurpressor){
        name = FString::Printf(TEXT("sound surpressor"));
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

void UWeaponContainer::updateGripName(){
    weaponAttachmentEnum type = setupinternal.getGripTypeToCreate();
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
///@brief auto finds attachment types and sets it
void UWeaponContainer::updateAnyWeaponAttachment(weaponAttachmentEnum type){
    if(weaponSetupHelper::isASightAttachment(type)){
        updateWeaponSight(type);
        return;
    }
    if(weaponSetupHelper::isAMuzzleAttachment(type)){
        updateWeaponMuzzle(type);
        return;
    }
    if(weaponSetupHelper::isAGripAttachment(type)){
        
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