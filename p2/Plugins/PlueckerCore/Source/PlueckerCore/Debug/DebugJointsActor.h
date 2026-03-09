#pragma once 

#include "CoreMinimal.h"
#include "PlueckerCore/Bone/Joint.h"
#include "PlueckerCore/Bone/RootJoint/RootJoint.h"
#include "CoreMath/animation/timer/Timer.h"
 
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
    bool isEnabled = true;

private:
    RootJoint rootJoint;

    void initChain();
    void LockPosition();
    void TickDebugRandomTorque(float deltaTime);

    Timer timerFortick;
    
    


};