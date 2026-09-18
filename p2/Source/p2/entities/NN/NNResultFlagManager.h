#pragma once

#include "CoreMinimal.h"
#include "CoreMath/util/Raycaster.h"


/// Will process result positions and place
/// flags (AFlagActor) at those.
/// (Is used to display NN ResultPositions from NN PathFinder Plugin,
/// prediction of player trajectories)

class AFlagActor;

class P2_API NNResultFlagManager {

public:
    NNResultFlagManager();
    ~NNResultFlagManager();


    NNResultFlagManager(const NNResultFlagManager &other);
    NNResultFlagManager &operator=(const NNResultFlagManager &other);

    //will project the positions to the ground
    void UpdateFlagPositions(const TArray<FVector> &positions, UWorld *world);

    //will update all flag positions and adjust the count to the predicted positions
    void UpdateFlagPositions(const TArray<FVector> &positions);

    void DrawFlagPositionsFrom(FVector actorLocation, UWorld *world);

protected:
    TArray<AFlagActor *> flags;
    Raycaster raycaster;

    void ClearFlags();
};