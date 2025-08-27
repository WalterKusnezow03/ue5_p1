#pragma once 

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/StringData/StorageInterfaceStringArray.h"

class P2_API WorldListStorageInterface : public StorageInterfaceStringArray {

public:
    WorldListStorageInterface();
    virtual ~WorldListStorageInterface() override;

    /// @brief saves the world list to storage, removes duplicates and empty strings!
    /// @param worldNames names
    void Save(TArray<FString> &worldNames);
    bool Load(TArray<FString> &worldNamesOut);

    /// @brief removes a world from storage, all data saved.
    void DeleteWorld(FString name);

private:
    FString makePath();

};