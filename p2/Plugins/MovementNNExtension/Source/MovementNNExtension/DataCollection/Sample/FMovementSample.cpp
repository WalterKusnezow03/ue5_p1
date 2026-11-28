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
    return numGroundTruthTokens * FMovementToken::NumFloats() * sizeof(float);
}

void FMovementSample::AppendGroundTruth(TArray<uint8> &buffer){
    PrepareFitData();
    MakeBinBufferFromFloats(groundTruthTokens, buffer);
}

// ---- REQUEST TO NN SIMPLE ACCESS ----

// ---- process result for game ----
void FMovementSample::ProcessFromPredictionBytes(const TArray<uint8> &buffer){
    if (buffer.Num() != ResultDataSizeBytes()){
        predictionTokens.Empty();
        UE_LOG(LogTemp, Error, TEXT("FMovementSample: Expected %d prediction bytes, got %d"), ResultDataSizeBytes(), buffer.Num());
        return;
    }
    TArray<float> floatBuffer;
    TemplateBufferStorageInterface::ConvertBufferRaw<uint8, float>(buffer, floatBuffer);
    ProcessFromPredictionFloats(floatBuffer);
}

void FMovementSample::ProcessFromPredictionFloats(const TArray<float> &buffer){
    predictionTokens.Empty();
    const int values = FMovementToken::NumFloats();
    const int expectedValues = numGroundTruthTokens * values;
    if (buffer.Num() != expectedValues){
        UE_LOG(LogTemp, Error, TEXT("FMovementSample: Expected %d prediction floats, got %d"), expectedValues, buffer.Num());
        return;
    }
    predictionTokens.SetNum(numGroundTruthTokens);
    int innerIndex = 0;
    for (int i = values; i <= buffer.Num(); i += values){
        FMovementToken &tokenForConstruct = predictionTokens[innerIndex];
        tokenForConstruct.Generate(buffer, i - 1);
        innerIndex++;
    }
}

// ---- process result for game ----



//to be implemented!
float FMovementSample::SimilarityOfSample(SampleInterface &other){
    if(FMovementSample *casted = TSampleCast<FMovementSample>(other)){
        //compare tokens linear
        float loss = Loss(*casted);
        
        float similarity = FMath::Clamp(1.0f - loss, 0.0f, 1.0f);
        return similarity;
    }
    return 0.0f;
}

float FMovementSample::Loss(FMovementSample &other){

    float distanceSampleInput = DistanceSquared(inputTokens, other.inputTokens);
    float distanceSampleGroundTruth = DistanceSquared(groundTruthTokens, other.groundTruthTokens);
    float lossAll = distanceSampleInput + distanceSampleGroundTruth;

    float maxLossPerToken = 10.0f;
    float maxLoss = (inputTokens.Num() + other.inputTokens.Num() + groundTruthTokens.Num() + other.groundTruthTokens.Num()) / maxLossPerToken;
    if(maxLoss > 0.0f){
        return lossAll / maxLoss;
    }
    return 1.0f;
}

float FMovementSample::DistanceSquared(
    const TArray<FMovementToken> &buffer,
    const TArray<FMovementToken> &other
){
    float distance = 0.0f;
    int minIndex = std::min(buffer.Num(), other.Num());
    for (int i = 0; i < minIndex; i++){
        distance += buffer[i].DistanceSquared(other[i]);
    }
    return distance;
}




void FMovementSample::EmbedTrajectories(
    const TArray<Trajectory> &buffer,
    const FVector &locationOfRequest
){
    requestLocation = locationOfRequest;
    //LogTrajectoryBuffer(buffer);

    //split into 4 channels
    //but each is a feature of (delta X, deltaY, deltaZ, deltaT)
    inputTokens.SetNum(numInputTokens);
    int diff = numInputTokens - buffer.Num();

    //fill missing tokens if diff > 0 (not enough tokens)
    for (int i = 0; i < diff; i++){
        if (i < inputTokens.Num()){
            inputTokens[i].Reset();
        }
    }

    //if there were tokens missing, start at offset
    //if too many trajectories in buffer: start from offset to end
    //as they are sorted in time [min, ..., currentTime]
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

void FMovementSample::LogTrajectoryBuffer(const TArray<Trajectory> &buffer){
    for (int i = 0; i < buffer.Num(); i++)
    {
        const Trajectory &current = buffer[i];
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMovementSample::Trajectory %d %s"), i, *current.GetPosition().ToString()
            )
        );
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


void FMovementSample::NotifyTrajectoriesFor(INNMovementInterface *interface) const{
    if(interface){
        if(predictionTokens.Num() <= 0){
            return;
        }

        TArray<FVector> constructed;
        ConstructResultBuffer(constructed);
        interface->ResponseNNMovementPositions(constructed);
    }
}

void FMovementSample::ConstructResultBuffer(TArray<FVector> &constructed) const {
    constructed.SetNum(predictionTokens.Num());
    for (int i = 0; i < predictionTokens.Num(); i++){

        //prev pos + token, or requestLocation as Fallback
        FVector pos = GetPrevPos(constructed, i, requestLocation);
        const FMovementToken &token = predictionTokens[i];
        FVector dir = token.GetDirection();

        //write new pos to buffer
        constructed[i] = pos + dir;
    }
}

FVector FMovementSample::GetPrevPos(
    const TArray<FVector> &buffer, 
    int index, 
    const FVector &fallback
) const {
    if(index > 0 && index < buffer.Num()){
        return buffer[index - 1];
    }
    return fallback;
}