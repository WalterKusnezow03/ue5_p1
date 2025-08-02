#pragma once

#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"

class P2_API WeaponAttachmentValidator {

public:
    static std::vector<weaponAttachmentEnum> validSights();
    static std::vector<weaponAttachmentEnum> validGrips();
    static std::vector<weaponAttachmentEnum> validMuzzleAttachments();

    static bool isASightAttachment(weaponAttachmentEnum type);
    static bool isAMuzzleAttachment(weaponAttachmentEnum type);
    static bool isAGripAttachment(weaponAttachmentEnum type);



};