#pragma once 

#include "CoreMinimal.h"
#include "Humanoid/Bone/Joint.h"
#include "GameCore/util/timer.h"
 
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
    void findErrorTwist(
        MMatrix &root, 
        MMatrix &end, 
        FVector &target, 
        float deltatime
    );

    timer timerFortick;
    TArray<Joint *> createdJoints;


    //for single chain debug
    FVector angularVelocity;
    FVector linearVelocity;
    FVector targetLocalPos;
};