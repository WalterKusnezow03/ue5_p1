#include "FPathFinderNNSampleSet.h"
#include "PathfinderNNExtension/Storage/FPathFinderNNSampleStorageInterface.h"
#include "DebugPlugin/DebugHelper.h"

FPathFinderNNSampleSet::FPathFinderNNSampleSet(){
    batchPrepared = false;
    changedData = false;
}

FPathFinderNNSampleSet::~FPathFinderNNSampleSet(){

}



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
    }
}

void FPathFinderNNSampleSet::ResizeAll(){
    for (int i = 0; i < cache.Num(); i++){
        FMeshedPolygonTrajectoryLayered &current = cache[i];
        current.ResizeGrid144();
    }
}



void FPathFinderNNSampleSet::EndSave(){
    if(changedData){
        //resizeAll
        ResizeAll();

        FPathFinderNNSampleStorageInterface interface;
        interface.Save(cache);
    }
}