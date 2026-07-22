#pragma once

#include "CoreMinimal.h"
#include "weaponPartEnum.h"
#include <map>


//extracts and holds all weapon parts
//-> are gettable as scene component or <T> for example skeletal mesh comp ptr for animations
class P2_API WeaponPartsCollection {

public:
    void OverrideAllByName(TArray<USkeletalMeshComponent *> &components);
    void OverrideAllByName(TArray<USceneComponent *> &components);
    void OverrideComponentByName(USceneComponent *comp);
    USceneComponent *FindComponent(EweaponPartEnum type);

    template <typename T> 
    T *TFindComponent(EweaponPartEnum type){
        if(USceneComponent *raw = FindComponent(type)){
            if(T *casted = Cast<T>(raw)){
                return casted;
            }
        }
        return nullptr;
    }

    void OverrideComponent(EweaponPartEnum type, USceneComponent *comp);
    
private:
    std::map<EweaponPartEnum, USceneComponent *> componentsMap;
    

};