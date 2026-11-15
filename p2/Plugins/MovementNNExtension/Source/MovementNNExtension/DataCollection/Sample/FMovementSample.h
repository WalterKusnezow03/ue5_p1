#pragma once

#include "CoreMinimal.h"

#include "NNCommunicationPlugin/Communication/Sample/Interface/SampleInterface.h"
#include "MovementNNExtension/DataCollection/Sample/Token/FMovementToken.h"
#include "ActorTracking/Tracking/TrajectoryCollection/Container/Trajectory.h"

class MOVEMENTNNEXTENSION_API FMovementSample : public SampleInterface {

public:
    void ResetSampleData() override;

    //storage access
    bool LoadFromBinary(TArray<uint8> &buffer, uint8 *&Ptr);
    void AppendAsBinary(TArray<uint8> &buffer);




    // ---- REQUEST TO NN SIMPLE ACCESS ----
    // prepare data (for netB resize 144 for example)
    virtual void PrepareFitData() override;

    virtual bool PrepareAppendRequestBinary(TArray<uint8> &buffer) override;
    virtual bool PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer) override;

    //onnx input
    virtual bool PrepareAppendRequestBinary(FONNXModelInput &input) override;

    //rename to ResultDataSizeBytes, not grid.
    //virtual int ResultDataSizeBytes() = 0; //ResultDataSizeBytes()
    virtual int ResultDataSizeBytes() override;

    virtual void AppendGroundTruth(TArray<uint8> &buffer)override;
    // ---- REQUEST TO NN SIMPLE ACCESS ----

    // ---- process result for game ----
    virtual void ProcessFromPredictionBytes(const TArray<uint8> &buffer) override;
    virtual void ProcessFromPredictionFloats(const TArray<float> &buffer) override;
    // ---- process result for game ----



    //to be implemented!
    virtual float SimilarityOfSample(SampleInterface &other) override;

    

    //HAS TO BE OVERRIDEN!
    virtual FString GetTypeString() override{
        return "MovementSample";
    }

    //HAS TO BE OVERRIDEN!
    virtual FString StorageFileName() override{
        return "MovementSampleBin";
    }


    //MovementPredictionTask API
    void EmbedTrajectories(const TArray<Trajectory> &buffer);

    //MovementGroundTruthUpdate API
    //ground truth values are only added if the buffer is not
    //filled yet
    void AddGroundTruthValue(const Trajectory &other);

    //MovementGroundTruthComplete API
    bool GroundTruthReady();

private:
    int numInputTokens = 40;
    TArray<FMovementToken> inputTokens;

    int numGroundTruthTokens = 5;
    TArray<FMovementToken> groundTruthTokens;

    TArray<FMovementToken> predictionTokens;

    void MakeBinBufferFromFloats(const TArray<FMovementToken> &tokens, TArray<uint8> &outBuffer);

    TArray<Trajectory> groundTruthRawBuffer;
    bool GroundTruthRawReady();
};