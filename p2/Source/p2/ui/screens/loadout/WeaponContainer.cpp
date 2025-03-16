

#include "Components/HorizontalBox.h"
#include "CoreMinimal.h"
#include "WeaponContainer.h"

void UWeaponContainer::init(){
    createLayout();
}

void UWeaponContainer::createLayout(){
    baseHorizontalBox = NewObject<UHorizontalBox>(this);
    if(baseHorizontalBox != nullptr){

        weaponPreviewImageButton = NewObject<UImageOverlayedButton>(this);
        if(weaponPreviewImageButton){
            weaponPreviewImageButton->init();

            //set on click later, weapon must know LoadoutScreen class to show weapon picker

            addToHorizontalBox(weaponPreviewImageButton);
        }

        attachmentSightImageButton = NewObject<UImageOverlayedButton>(this);
        if(attachmentSightImageButton){
            attachmentSightImageButton->init();

            //set on click later, weapon must know LoadoutScreen class to show attachment picker

            addToHorizontalBox(attachmentSightImageButton);
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
 * internal image and text updates
 */
void UWeaponContainer::updateWeaponName(){
    weaponEnum type = setupinternal.getWeaponTypeToCreate();
    FString name = FString::Printf(TEXT("<weapon name>"));
    if (type == weaponEnum::assaultRifle){
        name = FString::Printf(TEXT("assault rifle"));
    }
    if(type == weaponEnum::pistol){
        name = FString::Printf(TEXT("pistol"));
    }
    setTextWeapon(name);
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


/**
 * 
 * ----- CALLBACK API ------
 * 
 */
void UWeaponContainer::updateWeaponType(weaponEnum typeIn){
    setupinternal.setWeaponTypeToCreate(typeIn);
    updateWeaponName();
}

void UWeaponContainer::updateWeaponSight(weaponAttachmentEnum sightIn){
    setupinternal.setSightAttachment(sightIn);
}







