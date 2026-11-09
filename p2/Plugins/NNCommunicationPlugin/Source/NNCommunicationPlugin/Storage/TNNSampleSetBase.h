

#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"
#include "NNCommunicationPlugin/Communication/Sample/Interface/SampleInterface.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "NNCommunicationPlugin/LoadingScreen/LoadingScreenDispatcher.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"

#include "FNNSampleSetBase.h"

#include "TNNSampleSetStorageInterface.h"

//template sample set for any "SampleInterface"
//todo: TSubClassOf must be ok / assert
template<typename T>
class NNCOMMUNICATIONPLUGIN_API TNNSampleSetBase : public FNNSampleSetBase{
    // Compile-time check: T must inherit from SampleInterface
    /**
     * Compile-time check: T must inherit from SampleInterface
     * 
     * //these functions are not given in the SampleInterface - but must be implemented
     * //by any derived class.
     * LoadFromBinary(TArray<uint8>& buffer, uint8 *& Ptr)
     * AppendAsBinary(TArray<uint8>& buffer)
     * 
     * In any cas the Sample Interface class should not be, and can be used
     * purely since it has virtual functions.
     * 
     */
    static_assert(
        std::is_base_of_v<SampleInterface, T>, 
        "T must derive from SampleInterface"
    );

protected:
    TSet<int> markedForRemoval;
    int loadedSize = 0;

    TArray<T> cache;
    //no constructor, keep false false
    bool changedData = false;   //false

public:

    TNNSampleSetBase(){
        ExtractType();
        ExtractFileName();
    }

protected:
    FString typeSavedName = "NONE";
    void ExtractType(){
        T temp;
        typeSavedName = temp.GetTypeString();
    }

    FString fileNameSaved = "NONE";
    void ExtractFileName(){
        T temp;
        fileNameSaved = temp.StorageFileName();
    }

public:
    FString GetTypeString(){
        return typeSavedName;
    }

    // --- call here to load and save ---
    //virtual void Load() = 0; //calls on load once finished
    void Load() override {
        //todo: give a path name / or file name
        TNNSampleSetStorageInterface interface;
        TArray<uint8> Binary;
        if(interface.Load(Binary, fileNameSaved)){
            if(LoadFromBinary(Binary)){
                DebugHelper::logMessage(FString::Printf(TEXT("TNNSampleSetBase::Loaded %d"), this->cache.Num()));
                OnLoad();
                // CleanUpSimilarSamples();
            }
        }
    }

    void EndSave() override {
        OnSave(); //clean up samples before save

        TNNSampleSetStorageInterface interface;
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("TNNSampleSetBase::(%s)To Save Before %d After Clean %d"), 
                *fileNameSaved,
                this->Num(),
                this->NumCleanedUp()
            )
        );

        if(this->changedData){

            TArray<uint8> asBinary;
            AppendToBinary(asBinary);
            interface.Save(asBinary, fileNameSaved);

            DebugHelper::logMessage(FString::Printf(TEXT("TNNSampleSetBase Saved %d samples"), this->NumCleanedUp()));
        }else{
            DebugHelper::logMessage(FString::Printf(TEXT("TNNSampleSetBase Saved, data not changed")));
        }
        
        OnSaveFinalize();
    }

protected:
    virtual void OnLoad(){
        ResizeAll(); //resize all samples for correct nn usuage
    }

    virtual void OnSave(){
        ResizeAll(); //resize all samples for correct nn usuage
        FindCleanUpSimilarSamples(); //takes a while...
    }

    virtual void OnSaveFinalize(){
        ClearCache();
    }

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

    //for batch training
    void PrepareBatchBinary(TArray<uint8> &bytes) override {
        for (int i = 0; i < cache.Num(); i++){
            cache[i].PrepareRequestAndResultBatchBinary(bytes);
        }
        FString message = FString::Printf(TEXT("TNNSampleSetBase::Prepare Binary for Batch %d"), cache.Num());
        DebugHelper::logMessage(message);
        batchPrepared = true;
    }

    //overriden
    void AddSample(SampleInterface &interface) override {
        // cast down to T (which is necesarry to add to TArray buffer 
        // per value (object cut otherwise which is not wanted)). 
        if(interface.GetTypeString().Equals(typeSavedName)){
            T &casted = static_cast<T &>(interface);
            TAddSample(casted);
        }
    }

protected:
    void TAddSample(T &sample){
        //check for sample type
        if(!sample.GetTypeString().Equals(typeSavedName)){
            return;
        }

        cache.Add(sample);
        FString message = FString::Printf(TEXT("TNNSampleSetBase::AddSample %d"), cache.Num());
        DebugHelper::logMessage(message);
        changedData = true;
    }


    //resize all samples for correct nn usuage
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
            float maxLoss = 0.025f; //0.05 too high, halfs the set // use In 0..1
            markedForRemoval = FindMarkedForRemoval(maxLoss);
            
            
            //TSet<int> removedIndices = FindMarkedForRemoval(maxLoss);
            //FilterOut(removedIndices);

            //debug
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("TNNSampleSetBase::RemovedBySimilarity %d , maxloss %.2f"),
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



    //storage interface
    bool LoadFromBinary(TArray<uint8> &Bytes){
        //load count
        //load data
        if(Bytes.Num() > 0){
            uint8 *Ptr = Bytes.GetData();

            int32 count = 0;
            TemplateBufferStorageInterface::LoadCount(count, Ptr);
            this->loadedSize = count; //save for later sample reduction from added samples.

            if(count > 0){
                this->cache.Empty();
                this->cache.SetNum(count);
                for (int i = 0; i < count; i++){
                    T &current = this->cache[i];
                    current.LoadFromBinary(Bytes, Ptr);
                }
                DebugHelper::logMessage("TNNSampleSetBase loaded Bin data, count objects ", this->cache.Num());
                return true;
            }
        }
        return false;
    }

    //storage interface append
    void AppendToBinary(TArray<uint8> &Bytes){
        int leftOver = this->NumCleanedUp(); // cache.Num() - markedForRemoval.Num();
        TemplateBufferStorageInterface::AppendCount(leftOver, Bytes);
        for(int i = 0; i < this->cache.Num(); i++){
            if(!this->markedForRemoval.Contains(i)){ //O(1) lookup
                T &current = this->cache[i];
                current.AppendAsBinary(Bytes);
            }
        }

    }


    


};