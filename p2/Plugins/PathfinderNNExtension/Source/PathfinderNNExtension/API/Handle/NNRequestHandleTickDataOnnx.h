#include "NNRequestHandleTickData.h"


struct PATHFINDERNNEXTENSION_API FNNRequestHandleTickDataOnnx : public FNNRequestHandleTickData {

    
    bool bIsBusy = false;
    
    
    FNNRequestHandleTickDataOnnx(){
        useOnnxInput = true;
    }
};