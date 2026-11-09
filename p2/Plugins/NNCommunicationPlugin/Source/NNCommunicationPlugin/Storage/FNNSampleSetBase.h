#pragma once

#include "CoreMinimal.h"


// will define the base structure of a
// multi sample set class
// which allows to add samples to a batch
// and add it to storage
// Will allow to handle this class and any subclass by a raw ptr inside NNRequestHandle Base.
class NNCOMMUNICATIONPLUGIN_API FNNSampleSetBase {

public:
    virtual void AddSample(SampleInterface &sample) {};
    virtual void PrepareBatchBinary(TArray<uint8> &bytes) {};
    virtual void Load() {};
    virtual void EndSave() {};

    bool BatchPrepared(){
        return batchPrepared;
    }

protected:
    //no constructor, keep false false
    bool batchPrepared = false; //false

};