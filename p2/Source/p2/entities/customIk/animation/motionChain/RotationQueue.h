#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "CoreMath/animation/TransformInterpolator.h"
#include "CoreMath/animation/FRotatorInterpolator.h"
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

    
    //TransformInterpolator interpolator;
    FRotatorInterpolator interpolator;

    bool transitioning = false;
};