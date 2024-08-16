// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/weapon/weapon.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"


weaponSetupHelper::weaponSetupHelper()
{
    //default value to create
    typeToCreate = weaponEnum::assaultRifle;

    //default value for sights
    sightToSet = weaponSightEnum::enum_ironsight;
}

weaponSetupHelper::~weaponSetupHelper()
{
}

//SET ATTACHMENT SECTION

//set type section
void weaponSetupHelper::setWeaponTypeToCreate(weaponEnum typeIn){
    typeToCreate = typeIn;
}

/// @brief sets the targeted sight to set to a value
/// @param sightIn 
void weaponSetupHelper::setSightAttachment(weaponSightEnum sightIn){
    sightToSet = sightIn;
}





//APPLY ATTACHMENT SECTION

/// @brief returns which weapon should be created
/// @return type of the weapon from enum
weaponEnum weaponSetupHelper::getWeaponTypeToCreate(){
    return typeToCreate;
}

/// @brief will apply seelcted attachment to a weapon if not null
/// @param weaponIn 
void weaponSetupHelper::applyAttachments(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        //apply attachments such as sights etc...

        weaponIn->applySight(sightToSet);
    }
}