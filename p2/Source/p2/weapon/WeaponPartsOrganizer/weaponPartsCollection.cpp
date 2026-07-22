#include "weaponPartsCollection.h"








void WeaponPartsCollection::OverrideAllByName(TArray<USkeletalMeshComponent*> &components){
    for (int i = 0; i < components.Num(); i++){
        if(USceneComponent *comp = components[i]){
            OverrideComponentByName(comp);
        }
    }
}

void WeaponPartsCollection::OverrideAllByName(TArray<USceneComponent*> &components){
    for (int i = 0; i < components.Num(); i++){
        if(USceneComponent *comp = components[i]){
            OverrideComponentByName(comp);
        }
    }
}


void WeaponPartsCollection::OverrideComponentByName(USceneComponent *comp){
    if(comp != nullptr){
        FString name = comp->GetName();

        if (name.Contains("verschluss")){
            OverrideComponent(EweaponPartEnum::EVerschluss, comp);
        } else if(name.Contains("mag")){
            OverrideComponent(EweaponPartEnum::EMag, comp);
        }else if(name.Contains("gehaeuse") || name.Contains("gehause")){
            OverrideComponent(EweaponPartEnum::EGehaeuse, comp);
        }else if(name.Contains("muzzle")){
            OverrideComponent(EweaponPartEnum::EMuzzle, comp);
        }else if(name.Contains("grip")){
            OverrideComponent(EweaponPartEnum::EGrip, comp);
        }else if(name.Contains("sight") || name.Contains("sightsocket")){
            OverrideComponent(EweaponPartEnum::ESight, comp);
        }
    }
}

void WeaponPartsCollection::OverrideComponent(EweaponPartEnum type, USceneComponent *comp){
    if(comp != nullptr){
        componentsMap[type] = comp;
    }
}

USceneComponent *WeaponPartsCollection::FindComponent(EweaponPartEnum type){
    if(componentsMap.find(type) != componentsMap.end()){
        return componentsMap[type];
    }
    return nullptr;
}