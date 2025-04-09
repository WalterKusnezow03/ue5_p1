#pragma once

#include "CoreMinimal.h"

class P2_API DynamicRenderedObjectDebugger{
public:
    static void createDebugObject(UWorld *world);

private:
    static UStaticMeshComponent *findStaticMesh(AActor *actor);
    static AActor *spawnDebugActor(UWorld *world);
};