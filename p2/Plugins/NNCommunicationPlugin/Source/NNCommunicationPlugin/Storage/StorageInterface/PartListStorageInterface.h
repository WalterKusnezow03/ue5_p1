#pragma once 

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/StringData/StorageInterfaceStringArray.h"

class NNCOMMUNICATIONPLUGIN_API PartListStorageInterface : public StorageInterfaceStringArray {

public:
    PartListStorageInterface();
    virtual ~PartListStorageInterface() override;

    void Save();
    bool Load();

    FString AddNextFileName();
    FString AddFileNameIfNeeded(int index);

    int32 SizeList();

    TArray<FString> GetAllPartNames();

private:
    FString makePath();

    void AddFileName(int index);
    void AddFileName(FString nameIn);
    FString MakeFileName(int index);

    FString fileNameSavedBase = "NONE";
    TArray<FString> partNames;
};