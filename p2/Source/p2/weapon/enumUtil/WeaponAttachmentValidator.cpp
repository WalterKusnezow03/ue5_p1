#include "WeaponAttachmentValidator.h"



/// ---- filter data ----

std::vector<weaponAttachmentEnum> WeaponAttachmentValidator::validSights(){
    std::vector<weaponAttachmentEnum> output = {
        weaponAttachmentEnum::iron_sight,
        weaponAttachmentEnum::reddot
    };
    return output;
}
std::vector<weaponAttachmentEnum> WeaponAttachmentValidator::validGrips(){
    std::vector<weaponAttachmentEnum> output = {
        weaponAttachmentEnum::grip_vertical
    };
    return output;
}
std::vector<weaponAttachmentEnum> WeaponAttachmentValidator::validMuzzleAttachments(){
    std::vector<weaponAttachmentEnum> output = {
        weaponAttachmentEnum::muzzle_flashSurpressor,
        weaponAttachmentEnum::muzzle_SoundSurpressor
    };
    return output;
}


// ---- check data ----

bool WeaponAttachmentValidator::isASightAttachment(weaponAttachmentEnum type){
    std::vector<weaponAttachmentEnum> sights = validSights();
    for (int i = 0; i < sights.size(); i++){
        if(sights[i] == type){
            return true;
        }
    }
    return false;
}


bool WeaponAttachmentValidator::isAMuzzleAttachment(weaponAttachmentEnum type){
    std::vector<weaponAttachmentEnum> attachments = validMuzzleAttachments();
    for (int i = 0; i < attachments.size(); i++){
        if(attachments[i] == type){
            return true;
        }
    }
    return false;
}

bool WeaponAttachmentValidator::isAGripAttachment(weaponAttachmentEnum type){
    std::vector<weaponAttachmentEnum> attachments = validGrips();
    for (int i = 0; i < attachments.size(); i++){
        if(attachments[i] == type){
            return true;
        }
    }
    return false;
}