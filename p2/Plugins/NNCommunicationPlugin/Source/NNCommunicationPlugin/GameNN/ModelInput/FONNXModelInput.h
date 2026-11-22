#pragma once

#include "CoreMinimal.h"

/// @brief stores input tensor buffers in seperate Arrays
/// for simplicity in passing onto the ONNX Model
class NNCOMMUNICATIONPLUGIN_API FONNXModelInput {

public:
    /** auto increases buffer of tensors. Is safe with any index. 
     * index must be greater that 0, otherwise it will be 0. */
    TArray<uint8> &GetBufferForInputTensor(int index);

    void Empty();

private:
    TArray<TArray<uint8>> inputTensors;
};