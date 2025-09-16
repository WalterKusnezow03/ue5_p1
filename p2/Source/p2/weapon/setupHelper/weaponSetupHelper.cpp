// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/weapon/weapon.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"

#include "p2/weapon/enumUtil/WeaponAttachmentValidator.h"


weaponSetupHelper::weaponSetupHelper()
{
    //default value to create
    typeToCreate = weaponEnum::assaultRifle;

    //default value for sights
    sightToSet = weaponAttachmentEnum::iron_sight;
}

weaponSetupHelper::weaponSetupHelper(const weaponSetupHelper &other){
    if(this != &other){
        *this = other;
    }
}
weaponSetupHelper& weaponSetupHelper::operator=(const weaponSetupHelper &other){
    if(this == &other){
        return *this;
    }
    typeToCreate = other.typeToCreate;
    sightToSet = other.sightToSet;
    muzzleToSet = other.muzzleToSet;
    return *this;
}

weaponSetupHelper::~weaponSetupHelper()
{
}

bool weaponSetupHelper::isSame(weaponSetupHelper &other){
    return (other.typeToCreate == typeToCreate) &&
           (sightToSet == other.sightToSet) &&
           (muzzleToSet == other.muzzleToSet);
}

//SET ATTACHMENT SECTION

/// @brief sets the weapon (type) which should be created from the entity manager
/// @param typeIn 
void weaponSetupHelper::setWeaponTypeToCreate(weaponEnum typeIn){
    typeToCreate = typeIn;
}

/// @brief sets the targeted sight to set to a value
/// @param sightIn 
void weaponSetupHelper::setSightAttachment(weaponAttachmentEnum sightIn){
    if(WeaponAttachmentValidator::isASightAttachment(sightIn)){
        sightToSet = sightIn;
    }
}

void weaponSetupHelper::setMuzzleAttachment(weaponAttachmentEnum typein){
    if(WeaponAttachmentValidator::isAMuzzleAttachment(typein)){
        muzzleToSet = typein;
    }
}

void weaponSetupHelper::setGripAttachment(weaponAttachmentEnum typein){
    if(WeaponAttachmentValidator::isAGripAttachment(typein)){
        gripToSet = typein;
    }
}


void weaponSetupHelper::SetAnyAttachmentFilterInternal(weaponAttachmentEnum typeIn){
    //are filtered anyways and should NOT appear more than once in
    //Attachment validator!

    //all calls will never create any issues, only ONE will be true!
    setSightAttachment(typeIn);
    setMuzzleAttachment(typeIn);
    setGripAttachment(typeIn);
}

//APPLY ATTACHMENT SECTION

/// @brief returns which weapon should be created
/// @return type of the weapon from enum
weaponEnum weaponSetupHelper::getWeaponTypeToCreate(){
    return typeToCreate;
}

weaponAttachmentEnum weaponSetupHelper::getSightTypeToCreate(){
    return sightToSet;
}

weaponAttachmentEnum weaponSetupHelper::getMuzzleTypeToCreate(){
    return muzzleToSet;
}

weaponAttachmentEnum weaponSetupHelper::getGripTypeToCreate(){
    return gripToSet;
}

/// @brief will apply seelcted attachment to a weapon if not null
/// @param weaponIn 
void weaponSetupHelper::applyAttachments(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        //apply attachments such as sights etc...

        weaponIn->applySight(sightToSet);
        weaponIn->applyMuzzle(muzzleToSet);
        weaponIn->applyGrip(gripToSet);
    }
}
