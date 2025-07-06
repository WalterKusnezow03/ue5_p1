#include "TwoJointBone.h"
#include "IkHumanoidModell/Ik/solver/TwoBoneGeometricSolve.h"
#include "GameCore/DebugHelper.h"


TwoJointBone::TwoJointBone(){
    markedForTriangleFlip = false;
}

TwoJointBone::~TwoJointBone(){

}

void TwoJointBone::markTriangleFlipAsWantedForArms(){
    markedForTriangleFlip = true;
}

void TwoJointBone::setup(float a, float b, UWorld *world){
    worldPtr = world;

    a = std::abs(a);
    b = std::abs(b);

    FVector aVec(0, 0, -a);
    FVector bVec(0, 0, -b);

    t1.setTranslation(aVec);
    t2.setTranslation(bVec);

    aVec *= -1.0f;
    bVec *= -1.0f;
    t1Inv.setTranslation(aVec);
    t2Inv.setTranslation(bVec);

    length = a + b;

    //foot
    FVector foot(20, 0, 0);
    t3.setTranslation(foot);
    foot *= -1.0f;
    t3Inv.setTranslation(foot);
}

void TwoJointBone::resetRotations(){
    r1.makeIdentity();
    r2.makeIdentity();
    r1Inv.makeIdentity();
    r2Inv.makeIdentity();
}

void TwoJointBone::clampTarget(FVector &target){
    if(target.Size() > length){
        clampToFullMotionRangeCircle(target);
    }
}

void TwoJointBone::clampToFullMotionRangeCircle(FVector &target){
    target = target.GetSafeNormal() * length;
}

FVector TwoJointBone::outOfreachDistance(FVector &target){
    FVector outVector(0, 0, 0);
    if (target.Size() > length){
        float difference = target.Size() - length;
        outVector = target.GetSafeNormal() * difference;
    }
    return outVector;
}

void TwoJointBone::flipTriangleIfMarkedWanted(float &pitch1, float &pitch2){
    if(markedForTriangleFlip){
        pitch1 *= -1.0f;
        pitch2 *= -1.0f;
    }
}

void TwoJointBone::MoveToTarget(FVector target, MMatrix &world, float deltatime){
    MoveToTarget(target);
    buildForward(world, deltatime);

    //debug:
    if(false){
        FVector reached = EndEffectorRelativeLocation();
        DebugHelper::logMessage("debugKinematic forward kinematic targeted: ", target);
        DebugHelper::logMessage("debugKinematic forward kinematic reached: ", reached);
        DebugHelper::logMessage("debugKinematic forward kinematic -----");
    }
}

void TwoJointBone::MoveToTarget(FVector &target){
    resetRotations();

    //clamp to movement range
    clampTarget(target);

    //globale rotation
    FVector2D x(1.0f, 0.0f);
    FVector2D z(0.0f, -1.0f);//DOWN-Z
    MMatrix lookAtTarget = MMatrix::createRotatorFrom(target, x, z, true);
    r1 = lookAtTarget;

    //pitch anziehen
    float pitchHip = 0.0f;
    float pitchKnee = 0.0f;

    float distance = target.Size();
    TwoBoneGeometricSolve::createPitchAnglesFor(distance, pitchHip, pitchKnee, t1, t2);
    flipTriangleIfMarkedWanted(pitchHip, pitchKnee);

    r1.pitchRadAdd(pitchHip);
    r2.pitchRadAdd(pitchKnee);

}




void TwoJointBone::MoveToTargetInverse(FVector target, float deltatime){
    MoveToTargetInverse(target);

    //it is very important to clear the end effectors own rotation,
    //otherwise the trajectory is not correctly followed (remove any pitch)
    removeRotationFromEndEffector();

    buildBackward(endEffectorWorld, deltatime); //builded from end effector

    //debug:
    if(false){
        FVector reached = EndEffectorRelativeLocation() * -1.0f;
        DebugHelper::logMessage("debugKinematic backward kinematic targeted: ", target);
        DebugHelper::logMessage("debugKinematic backward kinematic reached: ", reached);
        DebugHelper::logMessage("debugKinematic backward kinematic -----");
    }
}

void TwoJointBone::MoveToTargetInverse(FVector &target){
    resetRotations();
    //clamp to movement range
    clampTarget(target); //ok kein fehler

    //globale rotation
    FVector2D x(1.0f, 0.0f);
    FVector2D z(0.0f, 1.0f); //UP-Z
    MMatrix lookAtTarget = MMatrix::createRotatorFrom(target, x, z, true);
    r2Inv = lookAtTarget; //r2 instead of r1

    //test look at matrix
    //es funktioniert einwandfrei!
    if(false){
        FVector boneAxisDefaultUp(0, 0, 1);
        boneAxisDefaultUp = lookAtTarget * boneAxisDefaultUp;
        boneAxisDefaultUp = boneAxisDefaultUp.GetSafeNormal() * target.Size();
        DebugHelper::logMessage("debugKinematic backward kinematic look result: ", boneAxisDefaultUp);
    }

    

    //pitch anziehen
    float pitchHip = 0.0f;
    float pitchKnee = 0.0f;
    float distance = target.Size();
    TwoBoneGeometricSolve::createPitchAnglesFor(distance, pitchHip, pitchKnee, t2, t1);
    flipTriangleIfMarkedWanted(pitchHip, pitchKnee);

    //Special for backwards inverse kinematic
    //because the building is inverse the triangle must be flipped
    //to look consistent with the end effector building
    pitchHip *= -1.0f;
    pitchKnee *= -1.0f;

    r2Inv.pitchRadAdd(pitchHip);
    r1Inv.pitchRadAdd(pitchKnee);

}

float TwoJointBone::lengthOfBone(){
    return length;
}

void TwoJointBone::buildForward(MMatrix &world, float deltatime){
    //M = R * T <-- lese richtung per joint
    MMatrix j1 = r1 * t1;
    MMatrix j2 = r2 * t2;
    MMatrix j3 = t3;

    //M = world * j1 * j2 * j3
    MMatrix j1World = world * j1;
    MMatrix j2World = j1World * j2;
    MMatrix j3World = j2World * j3;

    //update start and end effector
    startEffectorWorld = world;
    endEffectorWorld = j2World;
    draw(world, j1World, j2World, j3World, deltatime);
}

void TwoJointBone::buildBackward(MMatrix &world, float deltatime){

    MMatrix j0;
    MMatrix j1 = r2Inv * t2Inv; // foot to knee
    MMatrix j2 = r1Inv * t1Inv; // knee to hip

    MMatrix j0World = world * j0; //ohne world
    MMatrix j1World = j0World * j1;
    MMatrix j2World = j1World * j2;


    startEffectorWorld = j2World; //update start effector
    draw(world, j0World, j1World, j2World, deltatime);
}

void TwoJointBone::draw(MMatrix &world, MMatrix &a, MMatrix &b, MMatrix &c, float dt){
    FVector wT = world.getTranslation();
    FVector aT = a.getTranslation();
    FVector bT = b.getTranslation();
    FVector cT = c.getTranslation();

    dt *= 3.0f;

    DebugHelper::showLineBetween(worldPtr, FVector(0,0,0), wT, FColor::Green, dt);
    DebugHelper::showLineBetween(worldPtr, aT, wT, FColor::Red, dt);
    DebugHelper::showLineBetween(worldPtr, aT, bT, FColor::Blue, dt);
    DebugHelper::showLineBetween(worldPtr, bT, cT, FColor::Cyan, dt);
}


MMatrix TwoJointBone::StartEffector(){
    return startEffectorWorld;
}

MMatrix TwoJointBone::EndEffector(){
    return endEffectorWorld;
}

FVector TwoJointBone::EndEffectorLocation(){
    return endEffectorWorld.getTranslation();
}

/// @brief clear rotation before backwards kinematics, otherwise chain is
/// build in a wrong rotation left over from forward builds !
void TwoJointBone::removeRotationFromEndEffector(){
    FVector l = EndEffectorLocation();
    endEffectorWorld.makeIdentity();
    endEffectorWorld.setTranslation(l);
}

FVector TwoJointBone::EndEffectorRelativeLocation(){
    FVector B = endEffectorWorld.getTranslation();
    FVector A = startEffectorWorld.getTranslation();
    //AB = B - A
    return B - A;
}

//outside tick update no target
void TwoJointBone::TickBuildForward(MMatrix &world, float deltatime){
    buildForward(world, deltatime);
}