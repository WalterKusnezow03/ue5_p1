#pragma once

#include "CoreMinimal.h"

#include "PathfinderNNExtension/Interface/PathfinderNNHeatMapReceiver.h"


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
        for (int i = 0; i < listeners.Num(); i++){
            if(IPathfinderNNHeatMapReceiver *current = listeners[i]){
                current->SetLocation(worldPosPivot);
                current->ResponseHeatMap(image);
            }
        }
    }

};