#pragma once

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/VertexData/StorageInterface/enum/EFileType.h"

class STORAGEPLUGIN_API StorageInterface {

public:
    StorageInterface() {};
    virtual ~StorageInterface() {};

protected:
    bool SaveBinaryData(
        FString path,
        TArray<uint8> &Bytes
    );

    bool LoadBinaryData(
        FString Path,
        TArray<uint8> &Bytes
    );

};