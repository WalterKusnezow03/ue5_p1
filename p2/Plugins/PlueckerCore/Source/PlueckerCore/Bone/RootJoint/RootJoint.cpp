#include "RootJoint.h"
#include "PlueckerCore/Math/Matrix3x3.h"

RootJoint::RootJoint(){
    RemoveConstraints();
}

RootJoint::~RootJoint(){
    Joint::~Joint();
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
    

    float weight = 10000000.0f; //100000000.0f;
    float w1 = 1.0f / weight;
    interia.scale(weight, weight, weight);
    interiaInverse.scale(w1, w1, w1);

    //needed here: (? UNCLEAR)
    //recursive interia matrix,
    //also needed: Interia Sum!

    mass = 10.0f * 100.0f;
    SetInteriaMatrixSphere();
}








void RootJoint::TickAndBuildRecursive(
    float deltaTime
){
    MMatrix m; //none: stored in spatial transform

    //keep like this, current spatial velocity is copied in "tick build this joint"
    FVector w(0, 0, 0);
    FVector v(0, 0, 0);
    Joint::TickAndBuildRecursive(deltaTime, w, v, m);

    //debug log spatial velocity
    if(bLogEnabled){
        FString message = GetSpatialVelocity().ToString("RootJoint::SpatialVelocity");
        DebugHelper::showScreenMessage(message, FColor::Red);
    }
}



// override velocity on collapse hip
void RootJoint::OverrideLinearVelocity(FVector &vIn){
    GetSpatialVelocity().OverrideLinearVelocity(vIn);
}


void RootJoint::OverrideWorldLocation(MMatrix mat){
    OverrideWorldLocation(mat.getTranslation());
}

void RootJoint::OverrideWorldLocation(FVector pos){
    GetSpatialTransform().setTranslation(pos); //world location stored in spatial transform
}

FVector RootJoint::GetWorldLocation() const {
    return GetSpatialTransformConst().getTranslation();
}

void RootJoint::OverrideJointWorldTransform(FVector pos, FRotator rotation){
    OverrideJointRotation(rotation);
    OverrideWorldLocation(pos);
}





//never called. Unknown why. Bad.

//update com on backward build
void RootJoint::FindSelfInteriaAndGravitySpatialMoment(
    FVector &outN, // torque
    FVector &outF // force
){
    //DebugHelper::showScreenMessage("RootJoint FindSelfInteriaAndGravitySpatialMoment ", FColor::Orange);
    
    //use complete body center of mass for torque
    if(true){
        UpdateCenterOfMassOnBackwardBuildForce();
    }
    
    Joint::FindSelfInteriaAndGravitySpatialMoment(outN, outF);
    //...
    //centerOfMass---> update needed
    //FVector torqueExternal = GetSpatialTransform().Torque(forceExternal, centerOfMass);
    //...
}


void RootJoint::UpdateCenterOfMassOnBackwardBuildForce(){
    centerOfMass = CenterOfMassWorldWeightedRecursive();
    //DebugHelper::showScreenMessage("RootJoint Com ", centerOfMass, FColor::Orange);

    //draw
    if(bLogEnabled){
        FVector worldLocation = GetWorldLocation();
        DebugHelper::showLineBetween(
            world,
            worldLocation,
            worldLocation + centerOfMass,
            FColor::Orange,
            1.0f
        );
    }
}



