#pragma once

#include "PartListStorageInterface.h"

template <typename T>
class NNCOMMUNICATIONPLUGIN_API TPartListStorageInterface : public PartListStorageInterface{

public:
    
    TPartListStorageInterface<T>() : PartListStorageInterface(){
        ExtractFileName();
    }

    virtual ~TPartListStorageInterface<T>() override {

    }

protected:

    void ExtractFileName(){
        T sampleTemp;
        fileNameSavedBase = sampleTemp.StorageFileName();
    }
};