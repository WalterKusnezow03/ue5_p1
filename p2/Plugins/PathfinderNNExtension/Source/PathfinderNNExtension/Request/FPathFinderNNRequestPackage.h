#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"
#include "PathfinderNNExtension/Interface/VisionCone.h"

/// @brief package for task queue
class PATHFINDERNNEXTENSION_API FPathFinderNNRequestPackage {

public:
    FPathFinderNNRequestPackage();
    ~FPathFinderNNRequestPackage();

    FPathFinderNNRequestPackage(const FPathFinderNNRequestPackage &other);
    FPathFinderNNRequestPackage &operator=(const FPathFinderNNRequestPackage &other);

    FPathFinderNNRequestPackage(AActor *actorRequested);
    void Setup(AActor *actorRequested);
    bool IsSame(AActor *actorRequested);

    void Subscribe(IPathfinderNNInterface *interface);

    void NotifyAll(const TArray<FVector> &predictedPositions);

    AActor *GetActor();

    void GetRequesterVisionCones(TArray<FVisionCone *> &outPositions);


private:
    TArray<IPathfinderNNInterface *> subscribed;

    AActor *setupActor = nullptr;
};