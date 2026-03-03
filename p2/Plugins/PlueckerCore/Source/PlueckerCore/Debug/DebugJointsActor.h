#pragma once 

#include "CoreMinimal.h"
#include "PlueckerCore/Bone/Joint.h"
#include "CoreMath/animation/timer/timer.h"
 
#include "DebugJointsActor.generated.h"

class MMatrix;

UCLASS()
class PLUECKERCORE_API ADebugJointsActor : public AActor {

    GENERATED_BODY()

public:
    static void CreateInstance(UWorld *world, FVector &location);
    
    ADebugJointsActor();
    void BeginPlay() override;
    void Tick(float deltaTime) override;

protected:
    

private:
    Joint rootJoint;

    void initChain();
    void TickDebugRandomTorque(MMatrix &translation, float deltaTime);

    timer timerFortick;
    
    


    void debugSolveIk();
    void testForwardTmpPluecker();
};