#pragma once

#include "StoragePlugin/Storage/BaseInterface/StorageInterface.h"


/// @brief saves a array of string to storage
class STORAGEPLUGIN_API StorageInterfaceStringArray : public StorageInterface {

public:
    StorageInterfaceStringArray();
    virtual ~StorageInterfaceStringArray() override;

    ///saves a array of strings to storage - not filtered or unique, just saved!
    void Save(FString path, TArray<FString> &Names);
    bool Load(FString path, TArray<FString> &NamesOut);

    ///@brief creates a filtered array (a set) of strings without empty strings.
    TArray<FString> makeUniqueNonEmpty(TArray<FString> &worldNames);

    void Test();



    //single string load and save
    static void AppendString(
        TArray<uint8> &Bytes,
        FString &name
    );
    static FString loadString(uint8* &Ptr);

private:
    bool moreDebugLog = true;

    void writeStringCount(TArray<uint8> &Bytes, TArray<FString> &names);
    int readStringCount(uint8* &Ptr);

   
    

    static int bytesForChars(int num);

    void print(TArray<FString> &array, FString prefixInner);
};