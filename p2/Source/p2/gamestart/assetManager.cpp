// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/gamestart/assetManager.h"
#include "CoreMinimal.h"
#include "p2/gamestart/assetEnums/rooms/roomAssetEnum.h"

assetManager *assetManager::instancePointer = nullptr;

/// @brief you are not allowed to delete this pointer!
/// @return instance pointer
assetManager* assetManager::instance(){
    if(assetManager::instancePointer == nullptr){
        assetManager::instancePointer = new assetManager();
    }
    return assetManager::instancePointer;
}

assetManager::assetManager()
{
}

assetManager::~assetManager()
{
}

// --- room intern assets ---
UClass *assetManager::findBp(roomAssetEnum type){
    UClass *found = roomAssets.getBp(type);
    if(found != nullptr){
        return found;
    }
    return nullptr;
}

void assetManager::addBp(roomAssetEnum type, UClass *uclass){
    roomAssets.addBp(type, uclass);
}

// --- entity ---
UClass *assetManager::findBp(entityEnum type){
    UClass *found = entityAssets.getBp(type);
    return found;
}

void assetManager::addBp(entityEnum type, UClass *uclass){
    if(uclass != nullptr){
        entityAssets.addBp(type, uclass);
    }
}


// --- weapons ---
UClass *assetManager::findBp(weaponEnum type){
    UClass *found = weaponAssets.getBp(type);
    return found;
}

void assetManager::addBp(weaponEnum type, UClass *uclass){
    if(uclass != nullptr){
        weaponAssets.addBp(type, uclass);
    }
}

// --- throwables ---
UClass *assetManager::findBp(throwableEnum type){
    UClass *found = throwableAssets.getBp(type);
    return found;
}

void assetManager::addBp(throwableEnum type, UClass *uclass){
    if(uclass != nullptr){
        throwableAssets.addBp(type, uclass);
    }
}

// --- particles ---
UClass *assetManager::findBp(particleEnum type){
    UClass *found = particleAssets.getBp(type);
    if(found != nullptr){
        return found;
    }
    return nullptr;
}

void assetManager::addBp(particleEnum type, UClass *uclass){
    if(uclass != nullptr){
        particleAssets.addBp(type, uclass);
    }
}




// --- room assets ---







// --- other assets will follow here... ---


// --- material assets ---
UMaterial *assetManager::findMaterial(materialEnum type){
    UMaterial* material = materialAssets.getBp(type);
    return material;
}

void assetManager::addMaterial(materialEnum type, UMaterial *material){
    if(material != nullptr){
        materialAssets.addBp(type, material);
    }
}