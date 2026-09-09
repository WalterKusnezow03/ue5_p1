#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"



/// @brief setup class to store Type Samples and Tensor sizes to setup
/// ONNX Based Net
class PATHFINDERNNEXTENSION_API FONNXModelsetup {

public:
    FONNXModelsetup();
    FONNXModelsetup(EPolygonSampleType type);

    FString MakeFilePath();
    bool IsValid();

    int GetWidth();
    int GetHeight();
    int GetChannels();

private:
    void SetupFromType(EPolygonSampleType type);

    EPolygonSampleType savedType = EPolygonSampleType::none; //default
    int tensorWidth = 1;
    int tensorHeight = 1;
    int channels = 1;


    FString PluginDir(FString pluginName);

    void InitTensor(int W, int H, int channelsIn);
};
