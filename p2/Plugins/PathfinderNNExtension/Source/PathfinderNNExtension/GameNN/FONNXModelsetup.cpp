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
        //todo
    }

}

bool FONNXModelsetup::IsValid(){
    return savedType != EPolygonSampleType::none;
}

void FONNXModelsetup::InitTensor(int W, int H, int channelsIn){
    tensorWidth = W;
    tensorHeight = H;
    channels = channelsIn;
}

int FONNXModelsetup::GetWidth(){
    return tensorWidth;
}
int FONNXModelsetup::GetHeight(){
    return tensorHeight;
}
int FONNXModelsetup::GetChannels(){
    return channels;
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