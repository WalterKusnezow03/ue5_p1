#pragma once

#include "CoreMinimal.h"

/// @brief stores input tensor buffers in seperate Arrays
/// for simplicity in passing onto the ONNX Model
class PATHFINDERNNEXTENSION_API FONNXModelInput {

public:
    TArray<uint8> &GetBufferForInputTensor(int index);

    void Empty();

private:
    TArray<TArray<uint8>> inputTensors;
};