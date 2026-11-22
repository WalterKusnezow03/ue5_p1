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

/// @brief interface for responding to the requester with some data
/// can be derived and data casted to correct format (e.g. TArray<FVector> for positions).
class NNCOMMUNICATIONPLUGIN_API IBaseNNInterface
{
    GENERATED_BODY()

public:
    virtual void ResponseNNData(const TArray<uint8> &data) = 0;
    
};


//IPathfinderNNHeatMapReceiver