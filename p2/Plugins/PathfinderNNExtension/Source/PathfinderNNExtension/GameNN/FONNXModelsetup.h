#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"
#include "NNCommunicationPlugin/GameNN/Tensor/FTensorSetup.h"
#include "NNCommunicationPlugin/GameNN/FONNXModelsetupBase.h"


/// @brief setup class to store Type Samples and Tensor sizes to setup
/// ONNX Based Net
class NNCOMMUNICATIONPLUGIN_API FONNXModelsetup : public FONNXModelsetupBase{

public:
    FONNXModelsetup();
    FONNXModelsetup(EPolygonSampleType type);

    virtual FString MakeFilePath() override;
    bool IsValid();


protected:
    void SetupFromType(EPolygonSampleType type);
    EPolygonSampleType savedType = EPolygonSampleType::none; //default

    virtual void InitInputAndOutputNames() override;

};
