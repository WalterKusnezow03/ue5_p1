#pragma once
#include "NNRequestHandleTickDataBase.h"

struct NNCOMMUNICATIONPLUGIN_API FNNRequestHandleTickDataOnnx : public FNNRequestHandleTickDataBase {

    
    bool bIsBusy = false;
    
    
    FNNRequestHandleTickDataOnnx(){
        useOnnxInput = true;
    }
};