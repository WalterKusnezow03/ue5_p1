#include "FMovementONNXModelsetup.h"

FMovementONNXModelsetup::FMovementONNXModelsetup(){
    SetupOnConstruct(); //default
}


void FMovementONNXModelsetup::SetupOnConstruct(){
    InitInputAndOutputNames();
    // unklar noch!
    InitTensor(144, 144, 4); //diese informationen müssen in ein NN Property package (hier.)
}

bool FMovementONNXModelsetup::IsValid(){
    return true;
}

FString FMovementONNXModelsetup::MakeFilePath(){
    FString pluginName = "PathFinderNNExtension";
    FString fileName = ONNXFileName();
    savedFilePath = FONNXModelsetupBase::MakeFilePath(pluginName, fileName);
    return savedFilePath;
}

FString FMovementONNXModelsetup::ONNXFileName(){
    return "MovementONNX.onnx";
}

void FMovementONNXModelsetup::InitInputAndOutputNames(){
    InputNames = { "input" };
    OutputNames = { "output" };
}
