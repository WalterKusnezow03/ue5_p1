#include "ExternalActorSpawnCollectionStorageInterface.h"

void ExternalActorSpawnCollectionStorageInterface::Save(
    FString worldLevelName, 
    ExternalActorSpawnCollection &collection
){
    
    TArray<uint8> Bytes;
    collection.AppendAsBinary(Bytes);
    FString path = makePath(worldLevelName);
    
    //SaveBinaryData(FString Path, TArray<uint8> &Bytes)
    if(SaveBinaryData(path, Bytes)){
        DebugHelper::logMessage("Storage Interface ExternalActorSpawnCollectionStorageInterface Saved: ", path);
    }
}

bool ExternalActorSpawnCollectionStorageInterface::Load(
    FString worldLevelName, 
    ExternalActorSpawnCollection &collection
){
    
    FString path = makePath(worldLevelName);
    TArray<uint8> Bytes;
    if (!LoadBinaryData(path, Bytes))
    {
        FString message = FString::Printf(TEXT("Storage Interface ExternalActorSpawnCollectionStorageInterface ERROR LOADING BIN DATA (%s)"), *path);
        DebugHelper::logMessage(message);
        return false;
    }

    uint8 *Ptr = Bytes.GetData(); //global pointer for loading, because meshdata is concatenated

    if(collection.LoadFromBinary(Bytes, Ptr)){
        return true;
    }
    return false;
}

FString ExternalActorSpawnCollectionStorageInterface::makePath(
    FString worldLevelName
){
    FString fileName = FString::Printf(TEXT("Terrain/ExternalActorSpawnCollection.bin"));
    FString Path = BaseDir(worldLevelName) + fileName;
    return Path;
}



