
#include "RotationQueue.h"
#include "DebugPlugin/DebugHelper.h"




RotationQueue::RotationQueue(){

    //default setup
    FRotator none;
    addTarget(RotationStates::ENone, none);

    FRotator forward;
    forward.Pitch -= 90.0f; //clock wise
    addTarget(RotationStates::E90Front, forward);

    FRotator slight;
    slight.Pitch -= 20.0f; //clock wise
    addTarget(RotationStates::ESlightFront, slight);
}

RotationQueue::~RotationQueue(){

}

bool RotationQueue::isTransitioning(){
    return transitioning;
}

void RotationQueue::addTarget(RotationStates state, FRotator rotator){
    rotationStatesMap[state] = rotator;
}

bool RotationQueue::hasState(RotationStates state){
    return rotationStatesMap.find(state) != rotationStatesMap.end();
}


void RotationQueue::updateStateIfPossible(RotationStates state, float timeToEnd){
    if(!isTransitioning()){
        updateState(state, timeToEnd);
    }
}

void RotationQueue::updateState(RotationStates state, float timeToEnd){
    if(
        hasState(state) && 
        hasState(currentState) &&
        state != currentState
    ){
        FRotator currentRotation = rotationStatesMap[currentState];
        FRotator nextRotation = rotationStatesMap[state];
        currentState = state; //update

        interpolator.resetReachedflag();
        interpolator.setTarget(currentRotation, nextRotation, timeToEnd);

        transitioning = true;

        //interpolator.overrideStart(currentRotation);
        //interpolator.overrideTarget(nextRotation);
    }
}

void RotationQueue::forceSetState(RotationStates state){
    currentState = state;
    transitioning = false;
}

FRotator RotationQueue::Tick(float DeltaTime){
    FRotator rotator;
    if(
        //interpolator.hasTargetSetup() 
        interpolator.TargetSetupFlag()
        && isTransitioning()){

        rotator = interpolator.interpolate(DeltaTime);
        // interpolator.interpolateRotationOnly(DeltaTime);

        if(interpolator.hasReachedTarget()){
            //done
            transitioning = false;
        }
        return rotator;
    }
    //return state current if found
    if(hasState(currentState)){
        return rotationStatesMap[currentState];
    }

    return rotator;
}

void RotationQueue::TickRollPitch(MMatrix &currentOrient, float DeltaTime){
    if(interpolator.TargetSetupFlag() && isTransitioning()){
    //if(interpolator.hasTargetSetup() && isTransitioning()){
        //causes bad lag for reasons i dont know
        float yawCurrentRad = currentOrient.extractYawAngleRad();
        float yawCurrentDeg = MMatrix::degToRadian(yawCurrentRad);
        FRotator add = Tick(DeltaTime);
        add.Yaw = yawCurrentDeg;

        DebugHelper::logMessage("rotationInterpolator ", add);

        //lag cause ?
        currentOrient.setRotation(add);

    }
}
