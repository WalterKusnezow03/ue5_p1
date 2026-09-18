#include "FONNXModelsetup.h"

FONNXModelsetup::FONNXModelsetup(){
    SetupFromType(EPolygonSampleType::EMeshedPolygonTrajectoryLayered); //default
}

FONNXModelsetup::FONNXModelsetup(EPolygonSampleType type){
    SetupFromType(type);
}

void FONNXModelsetup::SetupFromType(EPolygonSampleType type){
    savedType = type;

    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
        //as in python defined
        InitTensor(144, 144, 4); //diese informationen müssen in ein NN Property package (hier.)
        return;
    }
    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
        //todo multi input

        return;
    }
}

bool FONNXModelsetup::IsValid(){
    return savedType != EPolygonSampleType::none;
}

void FONNXModelsetup::InitTensor(int W, int H, int channelsIn){
    FTensorSetup tensor;
    tensor.Setup(W, H, channelsIn);
    inputTensorsBlueprint.Add(tensor);
}

TArray<FTensorSetup> &FONNXModelsetup::GetInputTensors(){
    return inputTensorsBlueprint;
}

FString FONNXModelsetup::MakeFilePath(){
    FString pluginName = "PathFinderNNExtension";
    FString pluginDir = PluginDir(pluginName);

    
    //FString PythonONNXPart = FString::Printf(TEXT("Python/onnxExport/netB_ONNX.onnx"));
    FString fileName = EPolygonSampleTypeNameConversion::ONNXFileName(savedType);
    FString PythonONNXPart = FString::Printf(TEXT("Python/onnxExport/%s"), *fileName);
    return FPaths::Combine(pluginDir, PythonONNXPart);
}

FString FONNXModelsetup::PluginDir(FString pluginName){
    FString inner = pluginName; //refactured. // FString::Printf(TEXT("%s/Source/%s"), *pluginName, *pluginName);
    FString pluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        *inner
    );
    DebugHelper::logMessage("ONNXLoader::PluginDir --> ", pluginDir);
    return pluginDir;
}


void FONNXModelsetup::InitInputAndOutputNames(){
    if(savedType == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
        InputNames = { "input" };  //NetB Export
        OutputNames = { "output" }; //NetB Export
        return;
    }
    if(savedType == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
        //todo
        //named consistent as (0,1,..n)
        InputNames = { "input0", "intput1" };  //NetC Export
        OutputNames = { "output" }; //NetC Export
        return;
    }
}


std::vector<const char*> FONNXModelsetup::GetInputNames(){
    return InputNames;
}

std::vector<const char*> FONNXModelsetup::GetOutputNames(){
    return OutputNames;
}