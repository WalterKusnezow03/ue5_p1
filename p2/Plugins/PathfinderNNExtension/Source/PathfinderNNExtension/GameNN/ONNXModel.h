#pragma once

#include "CoreMinimal.h"
#include "onnxruntime_cxx_api.h" 
#include "PathfinderNNExtension/GameNN/FONNXModelsetup.h"
#include "PathfinderNNExtension/GameNN/Tensor/FTensorSetup.h"
#include "PathfinderNNExtension/GameNN/Tensor/FTensor.h"

/// @brief Will Support Multi or Single Input nets, with single output nets
///
/// CAUTION: MULTI TENSOR INPUT NOT SUPPORTED YET!
class PATHFINDERNNEXTENSION_API ONNXModel {
public:
    ONNXModel();
    ~ONNXModel();

    //using this because of prediction map
    bool RunModel(
        TArray<uint8> &bufferIn, // binary like python / shared mem
        TArray<float> &bufferPredictionOut
    );

    void LoadModel(FONNXModelsetup setup);

    bool RunModelAsync(TArray<uint8> &bufferIn, TFunction<void(TArray<float>)> OnComplete);

    bool WasLoaded();

protected:
    bool RunModel(
        TArray<float> &bufferIn,
        TArray<float> &bufferPredictionOut
    );
    bool RunModel(
        const TArrayView<float> &bufferIn,
        TArray<float> &bufferPredictionOut
    );

private:
    void Reset();

    bool Uint8ToFloatConvertable(const TArray<uint8> &buffer) const;

   
    
    void LoadModel(FString ModelPath);

    bool InitEnviroment(FString ModelPath);
    void InitInputAndOutPutNames();

    void InitTensors();
    void AddInputTensors(TArray<FTensorSetup> &tensorBluePrints);
    void AddInputTensor(FTensorSetup &other);
    //void AddInputTensor(int W, int H, int channels);

    
    
    
    


    //setup data
    FONNXModelsetup setupData;

    //flags
    bool bModelWasLoaded = false;

    //env
    //Ort::Env *env = nullptr;
    Ort::Session *Session = nullptr;
    

    //tensor init
    std::vector<Ort::Value> inputTensorsRaw;
    TArray<FTensor> inputTensors;
    Ort::MemoryInfo *memoryInfo = nullptr;

    // --- deprecated ---
    //std::vector<int64_t> inputDimensions; // = { 1, channels, H, W };
    //std::vector<float> inputTensorValues;//write to this ref data
    
    //Ort::Value inputTensor; //is not changed
    // --- deprecated ---


    std::vector<const char*> InputNames;
    std::vector<const char*> OutputNames;

    bool CopyDataToTensor(TArray<float> &bufferIn, int index);
    bool CopyDataToTensor(const TArrayView<float> &buffer, int index);

    bool Forward(std::vector<Ort::Value> &output);
    bool Forward(TArray<float> &bufferPredictionOut);
};