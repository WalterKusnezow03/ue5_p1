#pragma once

#include "TPathFinderNNSampleSet.h"
#include "StoragePlugin/Storage/BaseInterface/StorageInterface.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/NameConversion/EPolygonSampleTypeNameConversion.h"

class FMeshedPolygonTrajectoryLayeredInterface;

class PATHFINDERNNEXTENSION_API TPathFinderNNSampleStorageInterface : public StorageInterface {

public:
    TPathFinderNNSampleStorageInterface() {};
    virtual ~TPathFinderNNSampleStorageInterface() override {};

    //save bytes instantly by type sample
    void Save(TArray<uint8> &Bytes, EPolygonSampleType type){
        FString path = makePath(partName(type));
        SaveBinaryData(path, Bytes);
    }

    bool Load(TArray<uint8> &DataOut, EPolygonSampleType type){
        FString path = makePath(partName(type));
        if(!LoadBinaryData(path, DataOut)){
            DebugHelper::logMessage("TPathFinderNNSampleStorageInterface Failed Loading BIN!");
            return false;
        }
        return true;
    }

    FString partName(EPolygonSampleType type){
        return EPolygonSampleTypeNameConversion::StorageName(type);
    }

private:
    FString partnameFallback = "binary1";


    

    FString makePath(FString part){
        FString innerPath = FString::Printf(TEXT("%s.bin"), *part);
        FString outString = BaseDir("PathFinderNNSamples") + innerPath;
        return outString;
    }
};