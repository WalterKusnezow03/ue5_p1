#pragma once

#include "StoragePlugin/Storage/BaseInterface/StorageInterface.h"

class PATHFINDERNNEXTENSION_API TNNSampleSetStorageInterface : public StorageInterface {

public:
    TNNSampleSetStorageInterface() {};
    virtual ~TNNSampleSetStorageInterface() override {};

    //save bytes instantly by type sample
    void Save(TArray<uint8> &Bytes, FString partName){
        FString path = makePath(partName);
        SaveBinaryData(path, Bytes);
    }

    bool Load(TArray<uint8> &DataOut, FString partName){
        FString path = makePath(partName);
        if(!LoadBinaryData(path, DataOut)){
            DebugHelper::logMessage("TNNSampleSetStorageInterface Failed Loading BIN!");
            return false;
        }
        return true;
    }

private:

    FString makePath(FString part){
        FString innerPath = FString::Printf(TEXT("%s.bin"), *part);
        FString outString = BaseDir("PathFinderNNSamples") + innerPath; //change base path later
        return outString;
    }
};