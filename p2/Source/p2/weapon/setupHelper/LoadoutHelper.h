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

    void clear();
    void push_back(weaponSetupHelper &other);
    void replace(int index, weaponSetupHelper &other);

    std::vector<Aweapon *> spawnAllWeaponsAndApplyAttachments(UWorld *world);

    bool loadOutWasModified();

private:
    bool loadoutWasModifedFlag = false;
    std::vector<weaponSetupHelper> vec;
    //std::vector<bool> wasModifiedFlag;

    bool indexIsValid(int i);
};