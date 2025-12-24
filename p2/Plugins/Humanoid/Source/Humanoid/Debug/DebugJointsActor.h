#pragma once 

#include "CoreMinimal.h"
#include "Humanoid/Bone/Joint.h"
#include "CoreMath/animation/timer/timer.h"
 
#include "DebugJointsActor.generated.h"

UCLASS()
class HUMANOID_API ADebugJointsActor : public AActor {

    GENERATED_BODY()

public:
    static void CreateInstance(UWorld *world);
    
    ADebugJointsActor();
    void BeginPlay() override;
    void Tick(float deltaTime) override;

protected:
    virtual void BeginDestroy() override;

private:
    void initChain();
    

    timer timerFortick;
    TArray<Joint *> createdJoints;


    //for single chain debug
    FVector angularVelocity;
    FVector linearVelocity;

    FVector target;


    void debugSolveIk();
    void testForwardTmpPluecker();
};