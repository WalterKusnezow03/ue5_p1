#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/entities/customIk/animation/TargetInterpolator.h"
#include <map>
#include "RotationStates.h"

class P2_API RotationQueue {

public:
    RotationQueue();
    ~RotationQueue();

    bool isTransitioning();
	void addTarget(RotationStates state, FRotator rotator);

	void updateStateIfPossible(RotationStates state, float timeToEnd);
	void updateState(RotationStates state, float timeToEnd);

    FRotator Tick(float DeltaTime);

    bool hasState(RotationStates state);
    void TickRollPitch(MMatrix &currentOrient, float DeltaTime);

    void forceSetState(RotationStates state);

private:

    std::map<RotationStates, FRotator> rotationStatesMap;
    RotationStates currentState = RotationStates::ENone;

    TargetInterpolator interpolator;

    bool transitioning = false;
};