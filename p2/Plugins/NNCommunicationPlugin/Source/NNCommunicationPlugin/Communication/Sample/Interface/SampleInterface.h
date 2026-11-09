#pragma once

#include "CoreMinimal.h"

/// @brief base sample interface class.
class NNCOMMUNICATIONPLUGIN_API SampleInterface {

public:
    // ---- REQUEST TO NN SIMPLE ACCESS ----
    //prepare data (for netB resize 144 for example)
    virtual void PrepareFitData() {};

    virtual bool PrepareAppendRequestBinary(TArray<uint8> &buffer) = 0;
    virtual bool PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer) = 0;

    //onnx input
    virtual bool PrepareAppendRequestBinary(FONNXModelInput &input) = 0;

    //rename to ResultDataSizeBytes, not grid.
    virtual int ResultDataSizeBytes() = 0; //ResultDataSizeBytes()

    virtual void AppendGroundTruth(TArray<uint8> &buffer) = 0;
    // ---- REQUEST TO NN SIMPLE ACCESS ----

    // ---- process result for game ----
    virtual void ProcessFromPredictionBytes(const TArray<uint8> &buffer) = 0;
    virtual void ProcessFromPredictionFloats(const TArray<float> &buffer) = 0;
    // ---- process result for game ----


    //must be converted with static cast in derived class
    bool IsSimilar(SampleInterface &other, float lossMax){
        float similarityMin = 1.0f - lossMax;
        float similarity = SimilarityOfSample(other);
        return similarity >= similarityMin;
    }

    //to be implemented!
    virtual float SimilarityOfSample(SampleInterface &other) = 0;

    template <typename T>
    T *TSampleCast(SampleInterface &other){
        T temp;
        if (temp.GetTypeString().Equals(other.GetTypeString())){
            T* derived_ptr = static_cast<T*>(&other); // Downcast (unchecked)
            return derived_ptr;
        }
        return nullptr;
    }

    //HAS TO BE OVERRIDEN!
    virtual FString GetTypeString(){
        return "SampleInterfaceBase";
    }

    //HAS TO BE OVERRIDEN!
    virtual FString StorageFileName(){
        return "SampleInterfaceBaseBin";
    }
};
