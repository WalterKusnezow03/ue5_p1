#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"
#include "TPathFinderNNSampleSetStorageInterface.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"

//template sample set for any "FMeshedPolygonTrajectoryLayeredInterface"
//todo: TSubClassOf must be ok / assert
template<typename T>
class PATHFINDERNNEXTENSION_API TPathFinderNNSampleSet {
    // Compile-time check: T must inherit from FMeshedPolygonTrajectoryLayeredInterface
    static_assert(
        std::is_base_of_v<FMeshedPolygonTrajectoryLayeredInterface, T>, 
        "T must derive from FMeshedPolygonTrajectoryLayeredInterface"
    );

public:

    TPathFinderNNSampleSet(){
        ExtractType();
    }
private:
    EPolygonSampleType typeSaved = EPolygonSampleType::none;
    void ExtractType(){
        T temp;
        typeSaved = temp.GetType();
    }


public:
    EPolygonSampleType GetType(){
        return typeSaved;
    }

    // --- call here to load and save ---
    void Load(){
        //todo: give a path name / or file name
        TPathFinderNNSampleStorageInterface interface;
        TArray<uint8> Binary;
        if(interface.Load(Binary, GetType())){
            if(LoadFromBinary(Binary)){
                DebugHelper::logMessage(FString::Printf(TEXT("TPathFinderNNSampleSet::Loaded %d"), cache.Num()));
                ResizeAll();
                //CleanUpSimilarSamples();
                
            }
        }
    }

    void EndSave(){
        TPathFinderNNSampleStorageInterface interface;

        ResizeAll();
        FindCleanUpSimilarSamples(); //takes a while...
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("TPathFinderNNSampleSet::(%s)To Save Before %d After Clean %d"), 
                *interface.partName(GetType()),
                Num(),
                NumCleanedUp()
            )
        );

        if(changedData){
            //resizeAll
            //ResizeAll();
            //CleanUpSimilarSamples();

            TArray<uint8> asBinary;
            AppendToBinary(asBinary);

            
            interface.Save(asBinary, GetType());
        }

        ClearCache();
    }
    // --- call here to load and save ---


private:

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


    //todo: flag array for removed items.
    //empty all later.


    //storage interface
    bool LoadFromBinary(TArray<uint8> &Bytes){
        //load count
        //load data
        if(Bytes.Num() > 0){
            uint8 *Ptr = Bytes.GetData();

            int32 count = 0;
            TemplateBufferStorageInterface::LoadCount(count, Ptr);
            loadedSize = count; //save for later sample reduction from added samples.

            if(count > 0){
                cache.Empty();
                cache.SetNum(count);
                for (int i = 0; i < count; i++){
                    T &current = cache[i];
                    current.LoadFromBinary(Bytes, Ptr);
                }
                DebugHelper::logMessage("TPathFinderNNSampleStorageInterface loaded Bin data, polygons ", cache.Num());
                return true;
            }
        }
        return false;
    }

    //storage interface append
    void AppendToBinary(TArray<uint8> &Bytes){
        int leftOver = NumCleanedUp(); // cache.Num() - markedForRemoval.Num();
        TemplateBufferStorageInterface::AppendCount(leftOver, Bytes);
        for(int i = 0; i < cache.Num(); i++){
            if(!markedForRemoval.Contains(i)){ //O(1) lookup
                T &current = cache[i];
                current.AppendAsBinary(Bytes);
            }
        }

        //deprecated

        /*//append count
        TemplateBufferStorageInterface::AppendCount(cache.Num(), Bytes);

        //append all
        for (int i = 0; i < cache.Num(); i++){
            T &current = cache[i];
            current.AppendAsBinary(Bytes);
        }*/
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
        FString message = FString::Printf(TEXT("TPathFinderNNSampleSet::Prepare Binary for Batch %d"), cache.Num());
        DebugHelper::logMessage(message);
        batchPrepared = true;
    }

    void AddSample(T &sample){
        cache.Add(sample);
        FString message = FString::Printf(TEXT("TPathFinderNNSampleSet::AddSample %d"), cache.Num());
        DebugHelper::logMessage(message);
        changedData = true;
    }


    void TryAddSample(FMeshedPolygonTrajectoryLayeredInterface &sampleIn){
        if(sampleIn.GetType() == GetType()){
            AddSample(static_cast<T &>(sampleIn));
        }
    }



protected:
    TSet<int> markedForRemoval;
    int loadedSize = 0;

    TArray<T> cache;
    //no constructor, keep false false
    bool batchPrepared = false; //false
    bool changedData = false;   //false
    
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