#pragma once

#include "CoreMinimal.h"


class P2_API NNPathFinderProxy {

public:
        
    bool PredictNode(
        FVector playerPos,
        FVector playerDirection,
        float timeSinceSeen,
        FVector ownPositon,
        FRotator ownRotation,
        float radius,
        FVector &outPrediction,
        int botId
    );
        

private:
    void CollectNodePositionsLocalSpace(
        const FVector &pos,
        float radius,
        TArray<FVector> &outArray,
        FVector ownPositon,
        FRotator rotation
    );

    void CollectNodePositionsLocalSpace(
        const FVector &pos,
        float radius,
        TArray<FVector> &outArray,
        MMatrix &MInverse
    );

};