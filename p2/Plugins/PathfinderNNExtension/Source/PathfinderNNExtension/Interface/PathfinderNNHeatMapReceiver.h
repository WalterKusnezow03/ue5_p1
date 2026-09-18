// MyInteractable.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PathfinderNNHeatMapReceiver.generated.h"

UINTERFACE(MinimalAPI)
class UPathfinderNNHeatMapReceiver : public UInterface
{
    GENERATED_BODY()
};

/// @brief interface for responding to pathfinder node pridictions
class PATHFINDERNNEXTENSION_API IPathfinderNNHeatMapReceiver
{
    GENERATED_BODY()

public:
    virtual void ResponseHeatMap(const Image &image) = 0;
    virtual void SetLocation(const FVector &location) = 0;
};


//IPathfinderNNHeatMapReceiver