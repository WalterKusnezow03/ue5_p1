#include "WidgetEntityLootPayload.h"
#include "p2/weapon/weapon.h"

void UWidgetEntityLootPayload::Setup(Aweapon *weaponPtr){
    if(weaponPtr){
        Setup(
            weaponPtr->getBulletsInMag(),
            weaponPtr->weaponType(),
            weaponPtr->getAmmunitionType()
        );
    }
}


void UWidgetEntityLootPayload::Setup(
    int countIn, 
    weaponEnum typeWeaponIn,
    ammunitionEnum typeAmmunitionIn
){
    ammunition = VerifyAmmunitionCount(countIn);
    typeWeapon = typeWeaponIn;
    typeAmmunition = typeAmmunitionIn;
}

int UWidgetEntityLootPayload::VerifyAmmunitionCount(int countIn){
    if(countIn < 0){
        countIn = 1;
    }
    return countIn;
}


ammunitionEnum UWidgetEntityLootPayload::GetTypeAmmunition(){
    return typeAmmunition;
}

int UWidgetEntityLootPayload::GetAmmunitionAmount(){
    return ammunition;
}
