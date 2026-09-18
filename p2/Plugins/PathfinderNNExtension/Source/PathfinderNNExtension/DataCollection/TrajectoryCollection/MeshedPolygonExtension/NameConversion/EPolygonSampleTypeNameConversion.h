#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"

class PATHFINDERNNEXTENSION_API EPolygonSampleTypeNameConversion {

public:

    static FString StorageName(EPolygonSampleType type){
        if(type == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
            return "trajectoryLayeredUnetSamples";
        }
        if(type == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
            return "trajectoryLayeredRayModelSamples";
        }
        return "binary1";
    }

    static FString ONNXFileName(EPolygonSampleType type){
        if(type == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
            return "netB_ONNX.onnx";
        }
        if(type == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
            return "netC_ONNX.onnx";
        }
        return "none";
    }

    static int32 NNTypeInt(EPolygonSampleType type){
        if(type == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
            return 0;
        }
        if(type == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
            return 1;
        }
        return 0;
    }
    
    static void WriteNNTypeIntTo(EPolygonSampleType type, TArray<uint8> &buffer){
        int32 writeInt = NNTypeInt(type);
        TemplateBufferStorageInterface::TAppendSingleValue<int32>(
            writeInt,
            buffer
        );
    }

private:


};