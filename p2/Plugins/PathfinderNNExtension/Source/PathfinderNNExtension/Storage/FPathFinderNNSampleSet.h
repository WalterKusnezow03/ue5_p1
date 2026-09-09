#pragma once

#include "CoreMinimal.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/MeshedPolygonTrajectoryLayeredInterface.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryRayModel.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"

#include "PathfinderNNExtension/Storage/Template/TPathFinderNNSampleSet.h"

class PATHFINDERNNEXTENSION_API FPathFinderNNSampleSet
{

public:

    void Load();
    void EndSave();

    bool BatchPrepared(EPolygonSampleType type);

    //add / load
    void PrepareBatchBinary(TArray<uint8> &bytes, EPolygonSampleType type);
    void AddSample(FMeshedPolygonTrajectoryLayeredInterface &sample);

protected:
    TPathFinderNNSampleSet<FMeshedPolygonTrajectoryLayered> unetSamples;
    TPathFinderNNSampleSet<FMeshedPolygonTrajectoryRayModel> raymodelSamples;


};