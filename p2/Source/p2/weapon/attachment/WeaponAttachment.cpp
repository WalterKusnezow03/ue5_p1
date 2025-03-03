#include "CoreMinimal.h"
#include "WeaponAttachment.h"


WeaponAttachment::WeaponAttachment(){
    actor = nullptr;
}

WeaponAttachment::WeaponAttachment(const WeaponAttachment &other){
    actor = nullptr;
    *this = other;
}

WeaponAttachment &WeaponAttachment::operator=(const WeaponAttachment &other){
    if(this == &other){
        return *this;
    }
    actor = other.actor;
    offsetMatrixForActor = other.offsetMatrixForActor;
    return *this;
}

WeaponAttachment::~WeaponAttachment(){
    actor = nullptr;
}



/// @brief saves the actor and offset matrix for this attachment
/// @param actorin 
/// @param other 
void WeaponAttachment::setup(AActor *actorin, MMatrix &other){
    if(actorin != nullptr){
        actor = actorin;
    }
    offsetMatrixForActor = other;
}

AActor *WeaponAttachment::attachedActorPointer(){
    if(actor != nullptr){
        return actor;
    }
    return nullptr;
}
MMatrix &WeaponAttachment::offsetMatrix(){
    return offsetMatrixForActor;
}
