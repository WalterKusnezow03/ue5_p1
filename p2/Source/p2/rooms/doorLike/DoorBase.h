#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/animation/TargetInterpolator.h"
#include "p2/interfaces/Interactinterface.h"
#include "DoorBase.generated.h"


UCLASS()
class P2_API ADoorBase : public AcustomMeshActor , public IInteractinterface {//ggf interact interface
    GENERATED_BODY()

public:
    ADoorBase();

    static ADoorBase *Construct(UWorld *world, FVector &location);

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void open();
    void close();

    virtual void interact() override;

protected:
    virtual void initMesh();

    bool canChangeStateNow();

    bool bIsOpenState = false;

    TargetInterpolator interpolator; //es braucht eine rotator direction

    float timeOfAnimation = 0.5f;
};