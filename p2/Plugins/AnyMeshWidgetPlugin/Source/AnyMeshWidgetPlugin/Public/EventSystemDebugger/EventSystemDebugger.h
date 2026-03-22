// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "CoreMath/animation/timer/Timer.h"

#include "EventSystemDebugger.generated.h"


/// @brief widgets can be registered via AActor And widget name, and
/// events dispatched accordingly to registered widgets.
UCLASS()
class ANYMESHWIDGETPLUGIN_API AEventSystemDebugger : public AActor {
    GENERATED_BODY()

protected:
    static AEventSystemDebugger *instancePtr;

public:
    AEventSystemDebugger();

    static void MakeInstance(UWorld *world);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void FireDebugEventHaube();
    void FireDebugEventHeck();

    Timer timer;
    float intervall = 2.0f;

    int index = 0;
    int ampelIndex = 0;
};
