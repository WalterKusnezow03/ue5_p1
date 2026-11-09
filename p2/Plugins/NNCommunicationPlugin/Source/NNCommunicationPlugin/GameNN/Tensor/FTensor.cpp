#include "FTensor.h"


void FTensor::SetupFrom(const FTensorSetup &other){
    Setup(other.GetWidth(), other.GetHeight(), other.GetChannels());
}


void FTensor::Setup(int W, int H, int channelsIn){
    FTensorSetup::Setup(W, H, channelsIn);

    // 1. Die finale Shape definieren: [Batch, Channels, Height, Width]
    // Entspricht exakt: x.view(4, 142, 142) -> unsqueeze(0)
    //std::vector<int64_t> InputDims = { 1, 4, 142, 142 };
    //std::vector<int64_t> inputDimensions = { 1, channels, H, W };
    inputDimensions = { 1, channels, H, W };
    int BufferSize = 1 * channels * H * W;

    // 2. Den flachen C++ Buffer mit der passenden Gesamtgröße reservieren
    // 1 * 4 * 142 * 142 = 80656 Elemente
    inputTensorValues = std::vector<float>(BufferSize, 0.0f);
}



bool FTensor::CopyDataToTensor(TArray<float> &buffer){
    TArrayView<float> FullViewBuffer(buffer);
    return CopyDataToTensor(FullViewBuffer);
}

bool FTensor::CopyDataToTensor(const TArrayView<float> &buffer){
    
    if(buffer.Num() > 0 && buffer.Num() == inputTensorValues.size()){
        //copy buffer in into input tensor std vector
        //FMemory::Memcpy(*Target, *Src, sizeT)
        FMemory::Memcpy(
            inputTensorValues.data(),  // Ziel: Die direkte RAM-Adresse des std::vector, als ref in tensor drin!
            buffer.GetData(),          // Quelle: Die direkte RAM-Adresse des Unreal TArray
            buffer.Num() * sizeof(float) // Größe in Bytes: Anzahl der Floats * 4 Bytes
        );
        return true;
    }
    return false;
}

std::vector<int64_t> &FTensor::GetInputDimensions(){
    return inputDimensions;
}

std::vector<float> &FTensor::GetInputTensorValues(){
    return inputTensorValues;
}