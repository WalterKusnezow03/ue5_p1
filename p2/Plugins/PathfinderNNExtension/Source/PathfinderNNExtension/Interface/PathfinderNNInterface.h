// MyInteractable.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NNCommunicationPlugin/API/NotifyInterface/BaseNNInterface.h"
#include "VisionCone.h"
#include "PathfinderNNInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPathfinderNNInterface : public UBaseNNInterface
{
    GENERATED_BODY()
};

/// @brief interface for responding to pathfinder node pridictions
class PATHFINDERNNEXTENSION_API IPathfinderNNInterface : public IBaseNNInterface
{
    GENERATED_BODY()

public:
    virtual void ResponseNNPositions(const TArray<FVector> &positions) = 0;
    virtual void ResponseNNData(const TArray<uint8> &data) override {
        if(data.Num() % sizeof(FVector) != 0){
            UE_LOG(LogTemp, Warning, TEXT("IPathfinderNNInterface received byte data that is not a raw FVector array."));
            return;
        }

        TArray<FVector> positions;
        positions.SetNumUninitialized(data.Num() / sizeof(FVector));
        if(data.Num() > 0){
            FMemory::Memcpy(positions.GetData(), data.GetData(), data.Num());
        }
        ResponseNNPositions(positions);
    }

    virtual FVector GetWorldLocation() = 0;

    //todo
    virtual FVisionCone &GetVisionCone() = 0;
};


//IPathfinderNNHeatMapReceiver