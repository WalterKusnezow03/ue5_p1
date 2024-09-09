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

// --- room assets ---



// --- other assets will follow here... ---