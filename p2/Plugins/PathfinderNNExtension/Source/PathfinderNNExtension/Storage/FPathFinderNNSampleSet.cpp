#include "FPathFinderNNSampleSet.h"
#include "PathfinderNNExtension/Storage/FPathFinderNNSampleStorageInterface.h"
#include "DebugPlugin/DebugHelper.h"

FPathFinderNNSampleSet::FPathFinderNNSampleSet(){
    batchPrepared = false;
    changedData = false;
}

FPathFinderNNSampleSet::~FPathFinderNNSampleSet(){

}


/// @brief appends for each cached input and gt, the binary data!s
/// @param bytes 
void FPathFinderNNSampleSet::PrepareBinary(TArray<uint8> &bytes){
    for (int i = 0; i < cache.Num(); i++){
        cache[i].PrepareRequestAndResultBatchBinary(bytes);
    }
    FString message = FString::Printf(TEXT("FPathFinderNNSampleSet::Prepare Binary for Batch %d"), cache.Num());
    DebugHelper::logMessage(message);
    batchPrepared = true;
}

bool FPathFinderNNSampleSet::BatchPrepared(){
    return batchPrepared;
}

void FPathFinderNNSampleSet::AddSample(FMeshedPolygonTrajectoryLayered &sample){
    cache.Add(sample);
    FString message = FString::Printf(TEXT("FPathFinderNNSampleSet::AddSample %d"), cache.Num());
    DebugHelper::logMessage(message);
    DebugHelper::showScreenMessage(message);
    changedData = true;
}

void FPathFinderNNSampleSet::Load(){
    FPathFinderNNSampleStorageInterface interface;
    if(interface.Load(cache)){
        DebugHelper::logMessage(FString::Printf(TEXT("FPathFinderNNSampleSet::Loaded %d"), cache.Num()));

        ResizeAll();
        CleanUpSimilarSamples();
    }
}

void FPathFinderNNSampleSet::ResizeAll(){
    for (int i = 0; i < cache.Num(); i++){
        FMeshedPolygonTrajectoryLayered &current = cache[i];
        current.ResizeGrid144();
    }
}

//clean up in o(n^2)
void FPathFinderNNSampleSet::CleanUpSimilarSamples(){
    float maxLoss = 0.05f; // In 0..1
    TSet<int> removedIndices = FindMarkedForRemoval(maxLoss);
    FilterOut(removedIndices);
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FPathFinderNNSampleSet::RemovedBySimilarity %d , maxloss %.2f"),
            removedIndices.Num(),
            maxLoss
        )
    );
}

void FPathFinderNNSampleSet::FilterOut(const TSet<int> &removeIndices){
    int leftOver = cache.Num() - removeIndices.Num();
    TArray<FMeshedPolygonTrajectoryLayered> kept;
    
    if(leftOver > 0){
        kept.SetNum(leftOver);

        int innerIndex = 0;
        for (int i = 0; i < cache.Num(); i++){
            if(!removeIndices.Contains(i)){
                kept[innerIndex] = cache[i];
                innerIndex++;
            }
        }
    }

    cache = kept;
}



TSet<int> FPathFinderNNSampleSet::FindMarkedForRemoval(float maxLoss){
    TSet<int> removedIndices;
    maxLoss = FMath::Clamp(std::abs(maxLoss), 0.0f, 1.0f);
    for (int i = 0; i < cache.Num(); i++){
        if(!removedIndices.Contains(i)){
            FMeshedPolygonTrajectoryLayered &current = cache[i];
            //compare for others
            for (int j = 0; j < cache.Num(); j++){
                //if not removed yet and not self
                //compare for similarity, remove index if similar
                if(i != j){
                    if(!removedIndices.Contains(j)){
                        FMeshedPolygonTrajectoryLayered &other = cache[j];
                        if(current.IsSimilar(other, maxLoss)){
                            removedIndices.Add(j);
                        }
                    }
                }
            }
        }    
    }
    return removedIndices;
}

void FPathFinderNNSampleSet::EndSave(){
    if(changedData){
        //resizeAll
        ResizeAll();
        CleanUpSimilarSamples();

        FPathFinderNNSampleStorageInterface interface;
        interface.Save(cache);
    }
}