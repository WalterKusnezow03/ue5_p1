#pragma once

#include "CoreMinimal.h"
#include "onnxruntime_cxx_api.h" 


class PATHFINDERNNEXTENSION_API ONNXModel {
public:
    ONNXModel();
    ~ONNXModel();

    //using this because of prediction map
    bool RunModel(
        TArray<uint8> &bufferIn, // binary like python / shared mem
        TArray<float> &bufferPredictionOut
    );

    void LoadModel();

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

    FString MakePath();
    FString PluginDir(FString pluginName);
    
    void LoadModel(FString ModelPath);

    void InitEnviroment(FString ModelPath);
    void InitInputAndOutPutNames();
    void InitTensor();
    void InitTensor(int W, int H, int channels);

    
    
    
    std::vector<int64_t> inputDimensions; // = { 1, channels, H, W };
    std::vector<float> inputTensorValues;//write to this ref data

    //flags
    bool bModelWasLoaded = false;

    //env
    //Ort::Env *env = nullptr;
    Ort::Session *Session = nullptr;
    

    //tensor init 
    Ort::MemoryInfo *memoryInfo = nullptr;
    Ort::Value inputTensor;

    std::vector<const char*> InputNames;
    std::vector<const char*> OutputNames;

    bool CopyDataToTensor(TArray<float> &bufferIn);
    bool CopyDataToTensor(const TArrayView<float> &buffer);

    bool Forward(std::vector<Ort::Value> &output);
    bool Forward(TArray<float> &bufferPredictionOut);
};