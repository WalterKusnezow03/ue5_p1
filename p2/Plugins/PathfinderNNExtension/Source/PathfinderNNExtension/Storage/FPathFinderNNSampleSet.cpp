#include "FPathFinderNNSampleSet.h"

#include "DebugPlugin/DebugHelper.h"


/// @brief appends for each cached input and gt, the binary data!s
/// @param bytes 
void FPathFinderNNSampleSet::PrepareBatchBinary(TArray<uint8> &bytes, EPolygonSampleType type){
    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
        unetSamples.PrepareBatchBinary(bytes);
        return;
    }
    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
        raymodelSamples.PrepareBatchBinary(bytes);
        return;
    }
}

bool FPathFinderNNSampleSet::BatchPrepared(EPolygonSampleType type){
    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
        return unetSamples.BatchPrepared();
    }
    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
        return raymodelSamples.BatchPrepared();
    }
    return false;
}

void FPathFinderNNSampleSet::AddSample(FMeshedPolygonTrajectoryLayeredInterface &sample){
    unetSamples.TryAddSample(sample);
    raymodelSamples.TryAddSample(sample);

    /*if(sample.GetType() == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
        unetSamples.AddSample(sample);
    }
    if(sample.GetType() == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
        raymodelSamples.AddSample(sample);
    }*/
}

void FPathFinderNNSampleSet::Load(){
    unetSamples.Load();
    raymodelSamples.Load();
}

void FPathFinderNNSampleSet::EndSave(){
    unetSamples.EndSave();
    raymodelSamples.EndSave();
}