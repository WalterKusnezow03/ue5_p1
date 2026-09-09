#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"

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

private:


};