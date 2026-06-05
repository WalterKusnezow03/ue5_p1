#pragma once

#include "FPathFinderNNRequestPackage.h"

class PATHFINDERNNEXTENSION_API FPathFinderNNRequestQueue {

public:


    
    void Subscribe(IPathfinderNNInterface *interface, AActor *targetActor);

    //notifies the front task and pop fronts it since its finished.
    void NotifyPopFront(const TArray<FVector> &predictedPositions);

    bool HasTasks();

    AActor *frontActor();
    FPathFinderNNRequestPackage *frontPackage();

protected:
    TArray<FPathFinderNNRequestPackage> tasks;

    FPathFinderNNRequestPackage *Find(AActor *targetActor);
};