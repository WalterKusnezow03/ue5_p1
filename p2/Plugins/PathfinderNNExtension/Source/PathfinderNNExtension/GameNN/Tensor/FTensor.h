#pragma once

#include "FTensorSetup.h"

/// @brief class to store a actual tensor used by the onnx model
///
///  WILL HANDLE THE TENSOR ON ITS OWN!
class PATHFINDERNNEXTENSION_API FTensor : public FTensorSetup {

public:

    void SetupFrom(const FTensorSetup &other);
    virtual void Setup(int widthIn, int heightIn, int channelsIn) override;

    bool CopyDataToTensor(TArray<float> &buffer);
    bool CopyDataToTensor(const TArrayView<float> &buffer);

    std::vector<int64_t> &GetInputDimensions();
    std::vector<float> &GetInputTensorValues();

private:
    //moved from ONNX Net
    std::vector<int64_t> inputDimensions; // = { 1, channels, H, W };
    std::vector<float> inputTensorValues;//write to this ref data

    
    

};