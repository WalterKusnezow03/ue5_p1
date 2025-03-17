

#include "weaponSetupHelper.h"
#include "p2/util/TVector.h"
#include "p2/_world/worldLevel.h"
#include "p2/player/playerInventory.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/DebugHelper.h"
#include "LoadoutHelper.h"



LoadoutHelper::LoadoutHelper(){

}


LoadoutHelper::~LoadoutHelper(){

}

bool LoadoutHelper::loadOutWasModified(){
    return loadoutWasModifedFlag;
}

///@brief clears the vector.
void LoadoutHelper::clear(){
    vec.clear();
}

///@brief copies the passed weapon setuphelper
void LoadoutHelper::push_back(weaponSetupHelper &other){
    vec.push_back(other);
}

///@brief replaces an index, if index is valid, otherwise pushback
void LoadoutHelper::replace(int index, weaponSetupHelper &other){
    //prints properly
    FString message = FString::Printf(TEXT("loadoutLogMessage replace method called, index: %d"), index);
    DebugHelper::logMessage(message);
    if (indexIsValid(index))
    {
        if(!vec[index].isSame(other)){
            vec[index] = other;
            loadoutWasModifedFlag = true;
        }
    }
    else
    {
        push_back(other);
        loadoutWasModifedFlag = true;
    }
}

bool LoadoutHelper::indexIsValid(int i){
    return i >= 0 && i < vec.size();
}

std::vector<Aweapon *> LoadoutHelper::spawnAllWeaponsAndApplyAttachments(UWorld *world){
    loadoutWasModifedFlag = false;
    std::vector<Aweapon *> outweapons;
    if(world){
        EntityManager *entityManagerPointer = worldLevel::entityManager();
        if(entityManagerPointer != nullptr){
            for (int i = 0; i < vec.size(); i++){
                weaponSetupHelper &currentSetupHelper = vec[i];
                Aweapon *weapon = entityManagerPointer->spawnAweapon(world, &currentSetupHelper);
                if(weapon != nullptr){
                    outweapons.push_back(weapon);
                }
            }
        }
    }
    return outweapons;
}




