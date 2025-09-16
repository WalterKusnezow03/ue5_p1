#pragma once

#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"

/// @brief seperate filter class which devides all attachments in logic
class P2_API WeaponAttachmentValidator {

public:
    ///--- all types of enums will belong only to one logical group!! ---
    static std::vector<weaponAttachmentEnum> validSights();
    static std::vector<weaponAttachmentEnum> validGrips();
    static std::vector<weaponAttachmentEnum> validMuzzleAttachments();

    ///--- all types of enums will belong only to one logical group!! ---
    static bool isASightAttachment(weaponAttachmentEnum type);
    static bool isAMuzzleAttachment(weaponAttachmentEnum type);
    static bool isAGripAttachment(weaponAttachmentEnum type);



};