#pragma once

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/GameNN/Tensor/FTensorSetup.h"
#include "NNCommunicationPlugin/GameNN/FONNXModelsetupBase.h"


/// @brief setup class to store Type Samples and Tensor sizes to setup
/// ONNX Based Net
class NNCOMMUNICATIONPLUGIN_API FMovementONNXModelsetup : public FONNXModelsetupBase{

public:
    FMovementONNXModelsetup();

    virtual FString MakeFilePath() override;
    bool IsValid() override;


protected:
    void SetupOnConstruct();

    virtual void InitInputAndOutputNames() override;

    //some.onnx name must be returned
    FString ONNXFileName(); 
};
