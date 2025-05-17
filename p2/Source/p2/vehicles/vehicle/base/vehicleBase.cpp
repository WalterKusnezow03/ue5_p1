#include "vehicleBase.h"
#include "p2/meshgen/customMeshActorBase.h"

AvehicleBase::AvehicleBase() : AcustomMeshActorBase(){

}
    
void AvehicleBase::BeginPlay(){

}


void AvehicleBase::Tick(float deltatime){

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

    //normal.Z gibt winkel richtung an
    float sign = xAxis.X * localMovementDirectionNormalized.Y; // - ay * bx, rest fällt weg weil xAxis Y = 0 !
    slipAngle *= sign;
    return slipAngle;
}