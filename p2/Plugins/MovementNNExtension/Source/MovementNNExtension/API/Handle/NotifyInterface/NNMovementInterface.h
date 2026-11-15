// MyInteractable.h

#pragma once

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/API/NotifyInterface/BaseNNInterface.h"

#include "NNMovementInterface.generated.h"

UINTERFACE(MinimalAPI)
class UNNMovementInterface : public UBaseNNInterface
{
    GENERATED_BODY()
};

/// @brief interface for responding to pathfinder node pridictions
class MOVEMENTNNEXTENSION_API INNMovementInterface : public IBaseNNInterface
{
    GENERATED_BODY()

public:

    virtual void ResponseNNMovementPositions(const TArray<FVector> &positions) = 0;

    virtual void ResponseNNData(const TArray<uint8> &data) override {
        if(data.Num() % sizeof(FVector) != 0){
            UE_LOG(LogTemp, Warning, TEXT("INNMovementInterface received byte data that is not a raw FVector array."));
            return;
        }

        TArray<FVector> positions;
        positions.SetNumUninitialized(data.Num() / sizeof(FVector));
        if(data.Num() > 0){
            FMemory::Memcpy(positions.GetData(), data.GetData(), data.Num());
        }
        ResponseNNMovementPositions(positions);
    }
    
};


//IPathfinderNNHeatMapReceiver