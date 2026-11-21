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

    /** tries to find the current file name and return the next one if possible */
    bool GetNextFileName(FString name, FString &outName) const;

protected:
    FString makePath();

    void AddFileName(int index);
    void AddFileName(FString nameIn);
    FString MakeFileName(int index);

    FString fileNameSavedBase = "NONE";
    TArray<FString> partNames;

private:
    bool IsValidIndex(int i) const;
};