#pragma once


#include "weaponSetupHelper.h"
#include "GameCore/util/TVector.h"
#include "p2/weapon/weapon.h"

/**
 * will store the weapon steup helpers
 */
class P2_API LoadoutHelper{

public:
    LoadoutHelper();
    ~LoadoutHelper();

    /// @brief clears the weapon setup
    void clear();

    /// @brief adds a weapon setup
    void push_back(weaponSetupHelper &other);

    ///@brief replaces an index, if index is valid, otherwise pushback the new weapon setup
    void replace(int index, weaponSetupHelper &other);

    ///@brief spawns all weapons with the internal setups and returns a vector of the 
    ///spawned weapons
    std::vector<Aweapon *> spawnAllWeaponsAndApplyAttachments(UWorld *world);

    bool loadOutWasModified();

    weaponSetupHelper getIfValidIndex(int index);

private:
    bool loadoutWasModifedFlag = false;
    std::vector<weaponSetupHelper> vec;
    //std::vector<bool> wasModifiedFlag;

    bool indexIsValid(int i);
};