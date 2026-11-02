

#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"
#include "NNCommunicationPlugin/Communication/Sample/Interface/SampleInterface.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "NNCommunicationPlugin/LoadingScreen/LoadingScreenDispatcher.h"

//template sample set for any "SampleInterface"
//todo: TSubClassOf must be ok / assert
template<typename T>
class NNCOMMUNICATIONPLUGIN_API TNNSampleSetBase {
    // Compile-time check: T must inherit from SampleInterface
    static_assert(
        std::is_base_of_v<SampleInterface, T>, 
        "T must derive from SampleInterface"
    );

protected:
    TSet<int> markedForRemoval;
    int loadedSize = 0;

    TArray<T> cache;
    //no constructor, keep false false
    bool batchPrepared = false; //false
    bool changedData = false;   //false

public:

    TNNSampleSetBase(){
        ExtractType();
    }
protected:
    FString typeSavedName = "NONE";
    virtual void ExtractType(){
        T temp;
        typeSavedName = temp.GetTypeString();
    }


public:
    FString GetTypeString(){
        return typeSavedName;
    }

    // --- call here to load and save ---
    virtual void Load() = 0; //calls on load once finished

protected:
    virtual void OnLoad(){
        ResizeAll();
    }

    virtual void OnSave(){
        ResizeAll();
        FindCleanUpSimilarSamples(); //takes a while...
    }

    virtual void OnSaveFinalize(){
        ClearCache();
    }

public:

    // --- call here to load and save ---


protected:

    int Num(){
        return cache.Num();
    }

    int NumCleanedUp(){
        return Num() - markedForRemoval.Num();
    }

    void ClearCache(){
        //clean
        markedForRemoval.Empty();
        changedData = false;
        cache.Empty();
    }

public:

    bool BatchPrepared(){
        return batchPrepared;
    }

    //for batch training
    void PrepareBatchBinary(TArray<uint8> &bytes){
        for (int i = 0; i < cache.Num(); i++){
            cache[i].PrepareRequestAndResultBatchBinary(bytes);
        }
        FString message = FString::Printf(TEXT("TNNSampleSetBase::Prepare Binary for Batch %d"), cache.Num());
        DebugHelper::logMessage(message);
        batchPrepared = true;
    }

    void AddSample(T &sample){
        cache.Add(sample);
        FString message = FString::Printf(TEXT("TNNSampleSetBase::AddSample %d"), cache.Num());
        DebugHelper::logMessage(message);
        changedData = true;
    }


    void TryAddSample(SampleInterface &sampleIn){
        if(sampleIn.GetTypeString() == GetTypeString()){
            AddSample(static_cast<T &>(sampleIn));
        }
    }



protected:
    
    //resize / prepare all / fit size all (unet for example to reshape to 144,144 size)
    void ResizeAll(){
        for (int i = 0; i < cache.Num(); i++){
            T &current = cache[i];
            //current.ResizeGrid144();
            current.PrepareFitData(); //resize happens inside
        }
    } 
    


    
    //clean up samples
    void FindCleanUpSimilarSamples(){
        if(cache.Num() > 1){
            float maxLoss = 0.025f; //0.05 too hight, halfs the set // In 0..1
            markedForRemoval = FindMarkedForRemoval(maxLoss);
            
            
            //TSet<int> removedIndices = FindMarkedForRemoval(maxLoss);
            //FilterOut(removedIndices);

            //debug
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("TPathFinderNNSampleSet::RemovedBySimilarity %d , maxloss %.2f"),
                    markedForRemoval.Num(),
                    maxLoss
                )
            );
        }
    }

    //find all indices where the loss is not enough, resulting the sample to be removed
    //stored inside the index set which is returned
    TSet<int> FindMarkedForRemoval(float maxLoss){
        TSet<int> removedIndices;
        maxLoss = FMath::Clamp(std::abs(maxLoss), 0.0f, 1.0f);

        int difference = cache.Num() - loadedSize;

        //filter all samples which were newly added to the set
        for (int i = 0; i < cache.Num(); i++){
            //if not removed yet
            if(!removedIndices.Contains(i)){
                T &current = cache[i];
                //compare for others, from appended samples on (from loadedSize)
                for (int j = loadedSize; j < cache.Num(); j++){
                    //if not removed yet and not self
                    //compare for similarity, remove index if similar
                    if(i != j){
                        if(!removedIndices.Contains(j)){

                            int innerIndex = j - loadedSize;
                            LoadingScreenDispatcher::UpdateLoadingEntry(
                                "TNNSampleSetBase::cleanup",
                                FString::Printf(
                                    TEXT("Clean Up Samples %d of %d"),
                                    innerIndex,
                                    difference
                                )
                            );

                                T &other = cache[j];
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
    
    
    
    
    void FilterOut(const TSet<int> &removeIndices){
        if(removeIndices.Num() <= 0){
            return;
        }
        changedData = true;

        int leftOver = cache.Num() - removeIndices.Num();
        TArray<T> kept;
        
        //copy data which is not marked for removal.
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


};