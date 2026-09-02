#include "FPathFinderNNSampleStorageInterface.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"


FPathFinderNNSampleStorageInterface::FPathFinderNNSampleStorageInterface(){

}
FPathFinderNNSampleStorageInterface::~FPathFinderNNSampleStorageInterface(){

}

// todo here: set max mb size per file

void FPathFinderNNSampleStorageInterface::Save(
    TArray<FMeshedPolygonTrajectoryLayered> &data
){
    TArray<uint8> Bytes;
    FString path = makePath(partnameFallback);
    
    //append count
    TemplateBufferStorageInterface::AppendCount(data.Num(), Bytes);

    //append all
    for (int i = 0; i < data.Num(); i++){
        FMeshedPolygonTrajectoryLayered &current = data[i];
        current.AppendAsBinary(Bytes);
    }

    // save
    SaveBinaryData(path, Bytes);

    DebugHelper::logMessage("FPathFinderNNSampleStorageInterface Save Bin data, polygons ", data.Num());

}



/// todo here: load targeted MB size / or file count as single binary

bool FPathFinderNNSampleStorageInterface::Load(
    TArray<FMeshedPolygonTrajectoryLayered> &dataOut
){
    //load
    FString path = makePath(partnameFallback);
    TArray<uint8> Bytes;
    if(!LoadBinaryData(path, Bytes)){
        DebugHelper::logMessage("FPathFinderNNSampleStorageInterface Failed Loading BIN!");
        return false;
    }


    //assemble polygons
    if(Bytes.Num() > 0){
        uint8 *Ptr = Bytes.GetData();

        int32 count = 0;
        TemplateBufferStorageInterface::LoadCount(count, Ptr);
        if(count > 0){
            dataOut.Empty();
            dataOut.SetNum(count);
            for (int i = 0; i < count; i++){
                FMeshedPolygonTrajectoryLayered &current = dataOut[i];
                current.LoadFromBinary(Bytes, Ptr);
            }
            DebugHelper::logMessage("FPathFinderNNSampleStorageInterface loaded Bin data, polygons ", dataOut.Num());
            return true;
        }
    }
    return false;
}

FString FPathFinderNNSampleStorageInterface::makePath(FString part){
    FString innerPath = FString::Printf(TEXT("%s.bin"), *part);
    FString outString = BaseDir("PathFinderNNSamples") + innerPath;
    return outString;
}


