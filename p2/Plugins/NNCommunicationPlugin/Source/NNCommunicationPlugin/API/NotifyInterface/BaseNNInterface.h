// MyInteractable.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "BaseNNInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBaseNNInterface : public UInterface
{
    GENERATED_BODY()
};

/// @brief interface for responding to pathfinder node pridictions
class NNCOMMUNICATIONPLUGIN_API IBaseNNInterface
{
    GENERATED_BODY()

public:
    virtual void ResponseNNData(const TArray<uint8> &data) = 0;
    
};


//IPathfinderNNHeatMapReceiver