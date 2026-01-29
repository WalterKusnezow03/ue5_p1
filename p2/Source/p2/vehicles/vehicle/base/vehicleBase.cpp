#include "vehicleBase.h"
#include "DebugPlugin/DebugHelper.h"

AvehicleBase::AvehicleBase(){
    driverInterface = nullptr;
    PrimaryActorTick.bCanEverTick = true;
}

void AvehicleBase::BeginPlay(){
    copyTransformOnStart();
    driverInterface = nullptr;
    SetActorTickEnabled(true);
}

void AvehicleBase::copyTransformOnStart(){
    FVector currentLocation = GetActorLocation();
    translation.setTranslation(currentLocation);

    FRotator r = GetActorRotation();
    rotation.setRotation(r);
}

void AvehicleBase::Tick(float deltatime){
    Super::Tick(deltatime);
    processPlayerInputUnseatCheck(deltatime);
}

void AvehicleBase::processPlayerInputUnseatCheck(float deltatime){
    TickUnseatTimer(deltatime);
    if (driverInterface != nullptr)
    {
        //input
        InputContainer &inputRef = driverInterface->input();
        if(inputRef.interactKeyWasPressed()){
            if(canUnseat()){
                driverInterface = nullptr;
                resetUnseatTimer();
            }
        }
    }
}


MMatrix AvehicleBase::transform(){
    return translation * rotation; //M = T * R <-- lese richtung
}


FVector AvehicleBase::moveDirectionToLocalRotationSpace(FVector &dir){
    MMatrix rInverse = rotation;
    rInverse.transpose();
    return rInverse * dir;
}

FVector AvehicleBase::moveDirectionToWorldRotationSpace(FVector &dir){
    return rotation * dir;
}

float AvehicleBase::velocity(){
    return velocityWorld.Size();
}




float AvehicleBase::SlipAngle(){
    //find velocity and move to car rotation system
    FVector localMovementDirectionNormalized = velocityLocal.GetSafeNormal();

    //x axis ist immer forward
    FVector xAxis(1, 0, 0);
    float slipAngle = FVector::DotProduct(
        localMovementDirectionNormalized, 
        xAxis
    ); //muss noch signiert werden! 

    if(slipAngle <= 0.01f){
        return 0.0f;
    }

    //normal.Z gibt winkel richtung an
    float sign = xAxis.X * localMovementDirectionNormalized.Y; // - ay * bx, rest fällt weg weil xAxis Y = 0 !
   
    
    //cos(theta) = a dot b // wenn a und b normalisiert
    slipAngle = std::acosf(slipAngle);
    slipAngle *= sign;

    return slipAngle;

}





/**
 * 
 * driver interaction
 * 
 */
void AvehicleBase::seatAsDriver(ISteeringinterface *driverIn){
    if(driverIn != nullptr){
        driverInterface = driverIn;
        DebugHelper::showScreenMessage("DRIVER SEAT", FColor::Cyan);
        resetUnseatTimer();
    }
}

void AvehicleBase::resetUnseatTimer(){
    float time = 1.0f;
    unseatTimer.Begin(time);
}

void AvehicleBase::TickUnseatTimer(float deltaTime){
    unseatTimer.Tick(deltaTime);
}

bool AvehicleBase::canUnseat(){
    return unseatTimer.timesUp();
}
