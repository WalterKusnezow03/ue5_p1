#include "FONNXModelsetupBase.h"

FONNXModelsetupBase::FONNXModelsetupBase(){
    InitInputAndOutputNames();
}

bool FONNXModelsetupBase::IsValid(){
    //return savedType != EPolygonSampleType::none;
    return true;
}

void FONNXModelsetupBase::InitTensor(int W, int H, int channelsIn){
    FTensorSetup tensor;
    tensor.Setup(W, H, channelsIn);
    inputTensorsBlueprint.Add(tensor);
}

TArray<FTensorSetup> &FONNXModelsetupBase::GetInputTensors(){
    return inputTensorsBlueprint;
}


FString FONNXModelsetupBase::MakeFilePath(){
    return savedFilePath;
}

FString FONNXModelsetupBase::MakeFilePath(FString pluginName, FString fileName){
    //FString pluginName = "PathFinderNNExtension";
    FString pluginDir = PluginDir(pluginName);

    
    //FString PythonONNXPart = FString::Printf(TEXT("Python/onnxExport/netB_ONNX.onnx"));
    FString PythonONNXPart = FString::Printf(TEXT("Python/onnxExport/%s"), *fileName);
    return FPaths::Combine(pluginDir, PythonONNXPart);
}

FString FONNXModelsetupBase::PluginDir(FString pluginName){
    FString inner = pluginName; //refactured. // FString::Printf(TEXT("%s/Source/%s"), *pluginName, *pluginName);
    FString pluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        *inner
    );
    DebugHelper::logMessage("ONNXLoader::PluginDir --> ", pluginDir);
    return pluginDir;
}


void FONNXModelsetupBase::InitInputAndOutputNames(){
    InputNames = { "input" };  //NetB Export
    OutputNames = { "output" }; //NetB Export
}


std::vector<const char*> FONNXModelsetupBase::GetInputNames(){
    return InputNames;
}

std::vector<const char*> FONNXModelsetupBase::GetOutputNames(){
    return OutputNames;
}