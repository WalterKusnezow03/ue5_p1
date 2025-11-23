#pragma once

#include "CoreMinimal.h"


/// @brief buffer to store created data and flag for async completed
class COMPUTERVISIONPLUGIN_API BufferPackage {

public:
    BufferPackage();
    ~BufferPackage();
    BufferPackage(const BufferPackage &other);
    BufferPackage &operator=(const BufferPackage &other);




    bool &refFlagCompleted(){
        return asyncCompleted;
    }

    void *&refDataPtr(){
        return dataPtr;
    }

    uint8* DataPtrAsUint8(){
        return static_cast<uint8*>(dataPtr);
    }
    
    
    bool ProcessHasFinished();
    void MarkLaunchCopy();

    FString flags();

private:
    bool markedStarted = false;

    bool asyncCompleted = false;
    void *dataPtr = nullptr;
};