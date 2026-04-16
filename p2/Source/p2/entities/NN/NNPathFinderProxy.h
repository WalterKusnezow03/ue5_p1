#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/timer/Timer.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"


class P2_API NNPathFinderProxy {

public:
    NNPathFinderProxy();


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

    void DebugSaveImage(
        FVector playerPos,
        float radius
    );

    void DebugSaveImage(
        FVector playerPos,
        float radius,
        float deltatime
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

    Timer timer;
    FMeshedPolygon meshedPolygonStatic;

    void DebugSaveMeshedPolygonToStorage(
        FMeshedPolygon &polygon,
        FString nameImage
    );
};