#pragma once

#include "CoreMinimal.h"

#include "PathfinderNNExtension/Interface/PathfinderNNHeatMapReceiver.h"
#include "DebugPlugin/DebugHelper.h"

class PATHFINDERNNEXTENSION_API HeatMapReceivers {

private:
    TArray<IPathfinderNNHeatMapReceiver *> listeners;

public:

    void Subscribe(IPathfinderNNHeatMapReceiver *interfaceIn){
        if(interfaceIn){
            if(listeners.Contains(interfaceIn) == false){
                listeners.Add(interfaceIn);
            }
        }
    }

    void NotifyAll(Image &image, FVector &worldPosPivot){
        int notified = 0;
        for (int i = 0; i < listeners.Num(); i++)
        {
            if(IPathfinderNNHeatMapReceiver *current = listeners[i]){
                current->SetLocation(worldPosPivot);
                current->ResponseHeatMap(image);
                notified++;
            }
        }
        DebugHelper::logMessage(
            FString::Printf(TEXT("NNRequestHandle::HeatMapReceivers::NotifyHeatMapReceivers Notify %d"), notified)
        );
    }

};