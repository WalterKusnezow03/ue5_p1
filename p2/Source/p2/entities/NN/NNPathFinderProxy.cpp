#include "NNPathFinderProxy.h"
#include "PathFinder/pathFinding/PathFinder.h"



bool NNPathFinderProxy::PredictNode(
    FVector playerPos,
    FVector playerDirection,
    float timeSinceSeen,
    FVector ownPositon,
    FRotator ownRotation,
    float radius,
    FVector &outPrediction
){
    TArray<FVector> localNet;
    CollectNodePositionsLocalSpace(playerPos, radius, localNet, ownPositon, ownRotation);



    return false;
}

void NNPathFinderProxy::CollectNodePositionsLocalSpace(
    const FVector &pos, 
    float radius,
    TArray<FVector> &outArray,
    FVector ownPositon,
    FRotator rotation
){
    ownPositon *= -1.0f;
    MMatrix t1(ownPositon);

    MMatrix r1(rotation);
    r1.transpose();

    //M = T * R <-- lese richtung --
    //M^-1 = R^T * T^-1
    MMatrix m1 = r1 * t1;
    CollectNodePositionsLocalSpace(pos, radius, outArray, m1);
}

void NNPathFinderProxy::CollectNodePositionsLocalSpace(
    const FVector &pos, 
    float radius,
    TArray<FVector> &outArray,
    MMatrix &MInverse
){
    if(APathFinder *instance = APathFinder::instance()){
        instance->CollectNodePositions(pos, radius, outArray);
        
        for (int i = 0; i < outArray.Num(); i++){
            outArray[i] = MInverse * outArray[i];
        }
    }
}

