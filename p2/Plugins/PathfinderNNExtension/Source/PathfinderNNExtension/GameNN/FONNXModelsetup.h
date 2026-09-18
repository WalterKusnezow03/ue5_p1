#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"
#include "PathfinderNNExtension/GameNN/Tensor/FTensorSetup.h"


/// @brief setup class to store Type Samples and Tensor sizes to setup
/// ONNX Based Net
class PATHFINDERNNEXTENSION_API FONNXModelsetup {

public:
    FONNXModelsetup();
    FONNXModelsetup(EPolygonSampleType type);

    FString MakeFilePath();
    bool IsValid();

    TArray<FTensorSetup> &GetInputTensors();

    std::vector<const char*> GetInputNames();
    std::vector<const char*> GetOutputNames();

private:
    void SetupFromType(EPolygonSampleType type);
    FString PluginDir(FString pluginName);

    EPolygonSampleType savedType = EPolygonSampleType::none; //default
    
    //deprecated
    int tensorWidth = 1;
    int tensorHeight = 1;
    int channels = 1;
    
    void InitTensor(int W, int H, int channelsIn);
    TArray<FTensorSetup> inputTensorsBlueprint;


    

    
    void InitInputAndOutputNames();

    //todo
    std::vector<const char*> InputNames; //may have more than one!
    std::vector<const char*> OutputNames;


};
