#include "PositionCluster.h"


void PositionCluster::ClusterPositions(
    TArray<FVector> &data,
    float distance
){
    TArray<FCluster> clusters;
    ClusterPositions(data, clusters, distance);
    data.SetNum(clusters.Num());
    for (int i = 0; i < clusters.Num(); i++){
        FCluster &current = clusters[i];
        data[i] = current.Mean();
    }
}

void PositionCluster::ClusterPositions(
    const TArray<FVector> &data,
    TArray<FCluster> &outClusters,
    float distance
){
    float distance2 = distance * distance;
    if(data.Num() > 0){

        bool added = AddFirstPositon(data, outClusters);

        int start = added ? 1 : 0;
        for (int i = start; i < data.Num(); i++){
            const FVector &current = data[i];
            AddToCluster(current, outClusters, distance2);
        }
    }
}

void PositionCluster::AddToCluster(
    const FVector &current,
    TArray<FCluster> &outClusters,
    float distance2
){
    int clusterIndex = FindClosestCluster(current, outClusters, distance2);
    if(clusterIndex >= 0 && clusterIndex < outClusters.Num()){
        outClusters[clusterIndex].Add(current);
    }else{
        outClusters.SetNum(outClusters.Num() + 1);
        FCluster &last = outClusters.Last();
        last.Add(current);
    }
}

int PositionCluster::FindClosestCluster(
    const FVector &pos,
    TArray<FCluster> &outClusters,
    float distance2
){
    int index = -1;
    float minDistance = distance2;
    for (int i = 0; i < outClusters.Num(); i++){
        FCluster &currentCluster = outClusters[i];

        float distanceUpdate = currentCluster.Distance2(pos);
        if(distanceUpdate <= minDistance){
            index = i;
            minDistance = distanceUpdate;
        }
    }

    return index;
}

bool PositionCluster::AddFirstPositon(
    const TArray<FVector> &data,
    TArray<FCluster> &outClusters
){
    if(data.Num() > 0 && outClusters.Num() < 0){
        outClusters.SetNum(1);
        FCluster &first = outClusters[0];
        first.Add(data[0]);
    }
    return false;
}