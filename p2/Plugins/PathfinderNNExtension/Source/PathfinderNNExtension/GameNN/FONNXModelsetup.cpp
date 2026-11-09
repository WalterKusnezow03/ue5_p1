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


FString FONNXModelsetup::MakeFilePath(){
    FString pluginName = "PathFinderNNExtension";
    FString fileName = EPolygonSampleTypeNameConversion::ONNXFileName(savedType);
    savedFilePath = FONNXModelsetupBase::MakeFilePath(pluginName, fileName);
    return savedFilePath;
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
