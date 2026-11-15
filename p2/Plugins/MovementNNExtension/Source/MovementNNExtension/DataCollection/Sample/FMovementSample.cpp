#include "FMovementSample.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"

void FMovementSample::ResetSampleData(){
    SampleInterface::ResetSampleData();

    //empty buffer
    predictionTokens.Empty();
    inputTokens.Empty();
    groundTruthTokens.Empty();
    groundTruthRawBuffer.Empty();
}

// --- storage interface ---

bool FMovementSample::LoadFromBinary(TArray<uint8> &buffer, uint8 *&Ptr){
    if(TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
        return false;
    }

    TemplateBufferStorageInterface::TLoadBuffer<FMovementToken>(inputTokens, Ptr);

    if(TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
        return false;
    }

    TemplateBufferStorageInterface::TLoadBuffer<FMovementToken>(groundTruthTokens, Ptr);
    return true;
}

void FMovementSample::AppendAsBinary(TArray<uint8> &buffer){

    TemplateBufferStorageInterface::TAppendBuffer<FMovementToken>(inputTokens, buffer);
    TemplateBufferStorageInterface::TAppendBuffer<FMovementToken>(groundTruthTokens, buffer);

}
// --- storage interface ---


// --- inner converter to float ---
void FMovementSample::MakeBinBufferFromFloats(const TArray<FMovementToken> &tokens, TArray<uint8> &outBuffer){
    for (int i = 0; i < tokens.Num(); i++){
        const FMovementToken &current = tokens[i];
        current.AppendToBuffer(outBuffer);
    }
}
// --- inner converter to float ---


// ---- REQUEST TO NN SIMPLE ACCESS ----
//prepare data (for netB resize 144 for example)
void FMovementSample::PrepareFitData(){
    inputTokens.SetNum(numInputTokens);
    groundTruthTokens.SetNum(numGroundTruthTokens);
}

bool FMovementSample::PrepareAppendRequestBinary(TArray<uint8> &buffer){
    PrepareFitData();
    MakeBinBufferFromFloats(inputTokens, buffer);
    return true;
}

bool FMovementSample::PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer){
    TArray<uint8> tempBuffer;
    if(PrepareAppendRequestBinary(tempBuffer)){
        AppendGroundTruth(tempBuffer);
        buffer.Append(tempBuffer);
        return true;
    }
    return false;
}

//onnx input
bool FMovementSample::PrepareAppendRequestBinary(FONNXModelInput &input){
    //Unet FMovementSample Model has only one Input Tensor
    TArray<uint8> &buffer = input.GetBufferForInputTensor(0);
    return PrepareAppendRequestBinary(buffer);

    //return false;
}

int FMovementSample::ResultDataSizeBytes(){
    return FMovementToken::NumFloats() * sizeof(float);
    //return 0;
}

void FMovementSample::AppendGroundTruth(TArray<uint8> &buffer){
    PrepareFitData();
    MakeBinBufferFromFloats(groundTruthTokens, buffer);
}

// ---- REQUEST TO NN SIMPLE ACCESS ----

// ---- process result for game ----
void FMovementSample::ProcessFromPredictionBytes(const TArray<uint8> &buffer){
    TArray<float> floatBuffer;
    TemplateBufferStorageInterface::ConvertBufferRaw<uint8, float>(buffer, floatBuffer);
    ProcessFromPredictionFloats(floatBuffer);
}

void FMovementSample::ProcessFromPredictionFloats(const TArray<float> &buffer){
    predictionTokens.Empty();
    int values = 4;
    int count = buffer.Num() % values;
    if(count > 0){
        predictionTokens.SetNum(count);
        int innerIndex = 0;
        for (int i = values; i < buffer.Num(); i += values)
        {
            FMovementToken &tokenForConstruct = predictionTokens[innerIndex];
            tokenForConstruct.Generate(buffer, i);
            innerIndex++;
        }
    }
}

// ---- process result for game ----



//to be implemented!
float FMovementSample::SimilarityOfSample(SampleInterface &other){
    if(FMovementSample *casted = TSampleCast<FMovementSample>(other)){
        //compare tokens
    }
    return 0.0f;
}




void FMovementSample::EmbedTrajectories(const TArray<Trajectory> &buffer){
    //split into 3 channels
    //or 4, but each is a feature of (delta X, deltaY, deltaZ, deltaT)
    inputTokens.SetNum(numInputTokens);
    int diff = numInputTokens - buffer.Num();

    //fill missing tokens if diff > 0 (not enough tokens)
    for (int i = 0; i < diff; i++){
        if (i < inputTokens.Num()){
            inputTokens[i].Reset();
        }
    }

    //if there were tokens missing, start at offset
    int start = std::max(0, diff);
    int bufferIndex = 0;
    for (int i = start; i < inputTokens.Num(); i++){
        int prevBufferIndex = std::max(bufferIndex - 1, 0);

        const Trajectory &prevTrajectory = buffer[prevBufferIndex];
        const Trajectory &currentTrajectory = buffer[bufferIndex];


        FMovementToken &token = inputTokens[i];
        token.Generate(currentTrajectory, prevTrajectory);

        bufferIndex++;
    }
}


void FMovementSample::AddGroundTruthValue(const Trajectory &other){
    if(!GroundTruthRawReady()){
        groundTruthRawBuffer.Add(other);
    }
}

bool FMovementSample::GroundTruthReady(){
    //once enough samples are collected
        //the ground truth buffer gets updated
        //in this case, true is returned
    if(GroundTruthRawReady()){

        groundTruthTokens.SetNum(numGroundTruthTokens);

        for (int i = 0; i < groundTruthRawBuffer.Num(); i++){
            int prev = std::max(i - 1, 0);

            const Trajectory &prevTrajectory = groundTruthRawBuffer[prev];
            const Trajectory &currentTrajectory = groundTruthRawBuffer[i];

            FMovementToken &token = groundTruthTokens[i];
            token.Generate(currentTrajectory, prevTrajectory);
        }

        groundTruthRawBuffer.Empty();
        return true;
    }
    return false;
}

bool FMovementSample::GroundTruthRawReady(){
    //since we need one token more to compute the deltas
    //for n deltas, n+1 trajectories are needed
    return groundTruthRawBuffer.Num() + 1 >= numGroundTruthTokens;
}