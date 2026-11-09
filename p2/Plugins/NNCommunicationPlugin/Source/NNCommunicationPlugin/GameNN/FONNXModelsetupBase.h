#pragma once

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/GameNN/Tensor/FTensorSetup.h"


/// @brief setup class to store Type Samples and Tensor sizes to setup
/// ONNX Based Net
class NNCOMMUNICATIONPLUGIN_API FONNXModelsetupBase {

public:
    FONNXModelsetupBase();

    virtual FString MakeFilePath();
    FString MakeFilePath(FString pluginName, FString fileName);
    bool IsValid();

    TArray<FTensorSetup> &GetInputTensors();

    std::vector<const char*> GetInputNames();
    std::vector<const char*> GetOutputNames();

protected:
    FString PluginDir(FString pluginName);

  
    
    //deprecated
    int tensorWidth = 1;
    int tensorHeight = 1;
    int channels = 1;
    
    void InitTensor(int W, int H, int channelsIn);
    TArray<FTensorSetup> inputTensorsBlueprint;

    FString savedFilePath;

    virtual void InitInputAndOutputNames();

    //todo
    std::vector<const char*> InputNames; //may have more than one!
    std::vector<const char*> OutputNames;


};
