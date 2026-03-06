#pragma once

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/BaseInterfacePaths/StorageInterfacePaths.h"

class STORAGEPLUGIN_API StorageInterface : public StorageInterfacePaths{

public:
    StorageInterface() {};
    virtual ~StorageInterface() {};

    /// @brief removes a sub dir in saved folder. Start with the name, no prefix or trianling "/" needed.
    /// @param subDir 
    void RemoveSubDir(FString subDir);

protected:
    bool SubDirAllowed(FString subDir);

    bool SaveBinaryData(
        FString path,
        TArray<uint8> &Bytes
    );

    bool LoadBinaryData(
        FString Path,
        TArray<uint8> &Bytes
    );



    void PrintBinary(TArray<uint8> &bytes, FString message);
};