#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"

class ANNPathFinderSocket;

class PATHFINDERNNEXTENSION_API FPathFinderNNSampleSet
{

public:
    FPathFinderNNSampleSet();
    ~FPathFinderNNSampleSet();

    void Load();
    void EndSave();

    bool BatchPrepared();

    void PrepareBinary(TArray<uint8> &bytes);
    void AddSample(FMeshedPolygonTrajectoryLayered &sample);

protected:
    TArray<FMeshedPolygonTrajectoryLayered> cache;
    bool batchPrepared = false;

    bool changedData = false;
    void ResizeAll();
    



    //clean up samples
    void CleanUpSimilarSamples();
    TSet<int> FindMarkedForRemoval(float maxLoss);
    void FilterOut(const TSet<int> &removeIndices);
};