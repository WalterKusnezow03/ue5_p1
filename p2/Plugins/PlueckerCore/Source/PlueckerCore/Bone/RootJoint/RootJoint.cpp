#include "RootJoint.h"
#include "PlueckerCore/Math/Matrix3x3.h"

RootJoint::RootJoint(){
    RemoveConstraints();
}

RootJoint::~RootJoint(){

}

RootJoint::RootJoint(FVector location, UWorld *world){
    Setup(location, world);
    SetInteriaMatrixAuto();
    RemoveConstraints();
}

RootJoint::RootJoint(const RootJoint &other) : Joint(other) // Basisklasse kopieren
{
    if(this != &other){
        *this = other;
    }
}


RootJoint &RootJoint::operator=(const RootJoint &other){
    if(this != &other){
        Joint::operator=(other); // Basisklasse zuweisen

        rootTransform = other.rootTransform;
        RemoveConstraints();
    }
    return *this;
}

void RootJoint::RemoveConstraints(){
    FJointConstraint &constraint = GetConstraint();
    constraint.UnLockPositionConstraint();
}

void RootJoint::SetInteriaMatrixAuto(){
    interia.makeZero();
    interiaInverse.makeZero();
    SetInteriaMatrixAngularVelocityLocked();

    float weight = 100000000.0f;
    float w1 = 1.0f / weight;
    interia.scale(weight, weight, weight);
    interiaInverse.scale(w1, w1, w1);

    //needed here: 
    //recursive interia matrix,
    //also needed: Interia Sum!
}








void RootJoint::TickAndBuildRecursive(
    float deltaTime
){
    MMatrix m; //none: stored in spatial transform

    //keep like this, current spatial velocity is copied in "tick build this joint"
    FVector w(0, 0, 0);
    FVector v(0, 0, 0);
    Joint::TickAndBuildRecursive(deltaTime, w, v, m);
}



// override velocity on collapse hip
void RootJoint::OverrideLinearVelocity(FVector &vIn){
    spatialVelocity.OverrideLinearVelocity(vIn);
}


void RootJoint::OverrideWorldLocation(FVector pos){
    Joint::OverrideWorldLocation(pos);
    GetSpatialTransform().setTranslation(pos); //world location stored in spatial transform
}

void RootJoint::OverrideJointWorldTransform(FVector pos, FRotator rotation){
    OverrideJointRotation(rotation);
    OverrideWorldLocation(pos);
}
