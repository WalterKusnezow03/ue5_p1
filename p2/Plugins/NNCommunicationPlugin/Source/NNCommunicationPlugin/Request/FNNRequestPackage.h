#pragma once

#include "CoreMinimal.h"

#include "NNCommunicationPlugin/API/NotifyInterface/BaseNNInterface.h"

/// @brief package for task queue 


class NNCOMMUNICATIONPLUGIN_API FNNRequestPackage {

private:
    UObject *targetObject = nullptr;

public:

    void Subscribe(IBaseNNInterface *interface);

    void NotifyAll(const TArray<uint8> &predictedPositions);

    void Setup(FString name, UObject *newTargetObject = nullptr);
    bool IsSame(FString name);

    TArray<IBaseNNInterface *> &GetSubscribedActors();

    UObject *GetTargetObject() const { return targetObject; }

private:
    TArray<IBaseNNInterface *> subscribed;

    FString taskName = "NONE";

    
};