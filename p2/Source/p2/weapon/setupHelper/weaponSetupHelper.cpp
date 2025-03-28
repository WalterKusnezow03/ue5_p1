// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/weapon/weapon.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"


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
    if(isASightAttachment(sightIn)){
        sightToSet = sightIn;
    }
}

void weaponSetupHelper::setMuzzleAttachment(weaponAttachmentEnum typein){
    if(isAMuzzleAttachment(typein)){
        muzzleToSet = typein;
    }
}

void weaponSetupHelper::setGripAttachment(weaponAttachmentEnum typein){
    if(isAGripAttachment(typein)){
        gripToSet = typein;
    }
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


std::vector<weaponAttachmentEnum> weaponSetupHelper::validSights(){
    std::vector<weaponAttachmentEnum> output = {
        weaponAttachmentEnum::iron_sight,
        weaponAttachmentEnum::reddot
    };
    return output;
}
std::vector<weaponAttachmentEnum> weaponSetupHelper::validGrips(){
    std::vector<weaponAttachmentEnum> output = {
        weaponAttachmentEnum::grip_vertical
    };
    return output;
}
std::vector<weaponAttachmentEnum> weaponSetupHelper::validMuzzleAttachments(){
    std::vector<weaponAttachmentEnum> output = {
        weaponAttachmentEnum::muzzle_flashSurpressor,
        weaponAttachmentEnum::muzzle_SoundSurpressor
    };
    return output;
}




bool weaponSetupHelper::isASightAttachment(weaponAttachmentEnum type){
    std::vector<weaponAttachmentEnum> sights = validSights();
    for (int i = 0; i < sights.size(); i++){
        if(sights[i] == type){
            return true;
        }
    }
    return false;
}


bool weaponSetupHelper::isAMuzzleAttachment(weaponAttachmentEnum type){
    std::vector<weaponAttachmentEnum> attachments = validMuzzleAttachments();
    for (int i = 0; i < attachments.size(); i++){
        if(attachments[i] == type){
            return true;
        }
    }
    return false;
}

bool weaponSetupHelper::isAGripAttachment(weaponAttachmentEnum type){
    std::vector<weaponAttachmentEnum> attachments = validGrips();
    for (int i = 0; i < attachments.size(); i++){
        if(attachments[i] == type){
            return true;
        }
    }
    return false;
}