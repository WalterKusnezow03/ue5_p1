
#pragma once
#include "CoreMinimal.h"
#include "PathFinder/pathFinding/PathFinder.h"


class PATHFINDER_API raycastTask{

public:
    raycastTask();
    ~raycastTask();

    raycastTask(const raycastTask &other);
    raycastTask &operator=(const raycastTask & other);

    void setup(
        UWorld *world,
        PathFinderNode *aIn,
        PathFinderNode *bIn
    );

    void execute();

private:
    bool passTangentialCheck();
    bool syncRaycastCanSee();

    UWorld *worldPointer = nullptr;
    PathFinderNode *a = nullptr;
    PathFinderNode *b = nullptr;

    bool wasExecuted = false;

    FCriticalSection locker;
};