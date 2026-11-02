#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"
#include "TPathFinderNNSampleSetStorageInterface.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"

#include "NNCommunicationPlugin/Storage/TNNSampleSetBase.h"

//template sample set for any "FMeshedPolygonTrajectoryLayeredInterface"
//todo: TSubClassOf must be ok / assert
template<typename T>
class PATHFINDERNNEXTENSION_API TPathFinderNNSampleSet : public TNNSampleSetBase<T>{
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
    virtual void ExtractType() override{
        TNNSampleSetBase<T>::ExtractType();
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
                DebugHelper::logMessage(FString::Printf(TEXT("TPathFinderNNSampleSet::Loaded %d"), this->cache.Num()));
                TNNSampleSetBase<T>::OnLoad();
                // CleanUpSimilarSamples();
            }
        }
    }

    void EndSave(){
        TNNSampleSetBase<T>::OnSave(); //clean up

        TPathFinderNNSampleStorageInterface interface;
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("TPathFinderNNSampleSet::(%s)To Save Before %d After Clean %d"), 
                *interface.partName(GetType()),
                this->Num(),
                this->NumCleanedUp()
            )
        );

        if(this->changedData){
            //resizeAll
            //ResizeAll();
            //CleanUpSimilarSamples();

            TArray<uint8> asBinary;
            AppendToBinary(asBinary);

            
            interface.Save(asBinary, GetType());
        }

        TNNSampleSetBase<T>::OnSaveFinalize();
    }
    // --- call here to load and save ---


private:


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
            this->loadedSize = count; //save for later sample reduction from added samples.

            if(count > 0){
                this->cache.Empty();
                this->cache.SetNum(count);
                for (int i = 0; i < count; i++){
                    T &current = this->cache[i];
                    current.LoadFromBinary(Bytes, Ptr);
                }
                DebugHelper::logMessage("TPathFinderNNSampleStorageInterface loaded Bin data, polygons ", this->cache.Num());
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