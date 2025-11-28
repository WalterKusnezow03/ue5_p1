#pragma once 

#include "CoreMinimal.h"
#include "ComputerVisionPlugin/Public/ComputerVision/FeatureExtraction/ImagePatch.h"

#include "tensorflow/c/tf_tensor.h"
#include "tensorflow/c/c_api.h"

class COMPUTERVISIONPLUGIN_API GlobalFeatureVector {

public:
    GlobalFeatureVector() {};
    ~GlobalFeatureVector() {};
    void Init(TArray<ImagePatch> &images, int maxImages, int dimension);

private:

};