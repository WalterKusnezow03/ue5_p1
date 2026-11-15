#pragma once

#include "PartListStorageInterface.h"

template <typename T>
class NNCOMMUNICATIONPLUGIN_API TPartListStorageInterface : public PartListStorageInterface{

public:
    
    TPartListStorageInterface<T>() : PartListStorageInterface(){
        ExtractFileName();
    }

    virtual ~TPartListStorageInterface<T>() : ~PartListStorageInterface(){

    }

private:

    void ExtractFileName(){
        T sampleTemp;
        fileNameSavedBase = sampleTemp.GetFileNameBase();
    }
};