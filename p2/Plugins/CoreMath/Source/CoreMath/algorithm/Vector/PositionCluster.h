#pragma once

#include "CoreMinimal.h"
#include "FCluster.h"

class COREMATH_API PositionCluster {

public:
    
    void ClusterPositions(
        TArray<FVector> &data,
        float distance
    );


    void ClusterPositions(
        const TArray<FVector> &data,
        TArray<FCluster> &outClusters,
        float distance
    );

private:
    void AddToCluster(
        const FVector &current,
        TArray<FCluster> &outClusters,
        float distance2
    );

    int FindClosestCluster(
        const FVector &pos,
        TArray<FCluster> &outClusters,
        float distance2
    );

    bool AddFirstPositon(
        const TArray<FVector> &data,
        TArray<FCluster> &outClusters
    );
};