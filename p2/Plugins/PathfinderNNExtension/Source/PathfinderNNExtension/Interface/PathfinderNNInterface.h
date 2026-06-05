// MyInteractable.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VisionCone.h"
#include "PathfinderNNInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPathfinderNNInterface : public UInterface
{
    GENERATED_BODY()
};

/// @brief interface for responding to pathfinder node pridictions
class PATHFINDERNNEXTENSION_API IPathfinderNNInterface
{
    GENERATED_BODY()

public:
    virtual void ResponseNNPositions(const TArray<FVector> &positions) = 0;
    virtual FVector GetWorldLocation() = 0;

    //todo
    virtual FVisionCone &GetVisionCone() = 0;
};