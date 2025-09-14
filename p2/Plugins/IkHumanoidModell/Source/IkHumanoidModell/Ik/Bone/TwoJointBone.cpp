#include "TwoJointBone.h"
#include "IkHumanoidModell/Ik/solver/TwoBoneGeometricSolve.h"
#include "GameCore/DebugHelper.h"

#include "IkHumanoidModell/actor/debugLimbs/CubeLimbMaker.h"


TwoJointBone::TwoJointBone(){
    markedForTriangleFlip = false;
}

TwoJointBone::~TwoJointBone(){

}

void TwoJointBone::markTriangleFlipAsWantedForArms(){
    markedForTriangleFlip = true;

    markedForTriangleflipLegs = false;
    markedForTriangleflipArms = true;
}

//new
void TwoJointBone::markTriangleFlipAsWantedForLegs(){
    markedForTriangleflipLegs = true;
    markedForTriangleflipArms = false;
}



void TwoJointBone::setup(float a, float b, UWorld *world){
    worldPtr = world;

    a = std::abs(a);
    b = std::abs(b);
    length = a + b;

    FVector aVec(0, 0, -a);
    FVector bVec(0, 0, -b);

    t1.setTranslation(aVec);
    t2.setTranslation(bVec);

    aVec *= -1.0f;
    bVec *= -1.0f;
    t1Inv.setTranslation(aVec);
    t2Inv.setTranslation(bVec);

   

    //foot
    FVector foot(20, 0, 0);
    t3.setTranslation(foot);
    foot *= -1.0f;
    t3Inv.setTranslation(foot);


    //create debug limbs (cubes)
    createLimbsIfNeeded(world, a, b);
}

void TwoJointBone::ResetAndRebuild(MMatrix &worldMatrix){
    resetRotations();
    TickBuildForward(worldMatrix, 0.0f);
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

bool TwoJointBone::targetIsInMotionCircle(FVector &target){
    return target.Size() < length;
}

void TwoJointBone::flipTriangleIfMarkedWanted(float &pitch1, float &pitch2){
    
    //not tested new code!
    //legs, genau überlegt
    if(markedForTriangleflipLegs){
        //only pos on hip, neg knee (drehrichtung knie beachten, pos gegen uhrzeiger sinn!)
        pitch1 = std::abs(pitch1)* -1.0f;
        pitch2 = std::abs(pitch2);
        return;
    }

    //---- temporary! - nicht genau überlegt ----
    //arme naturel nach unten
    if(markedForTriangleflipArms){
        //only neg on shoulder, neg ellbow (drehrichtung knie beachten, pos gegen uhrzeiger sinn!)
        pitch1 = std::abs(pitch1) * -1.0f;
        pitch2 = std::abs(pitch2);
        return;
    }


    //deprecated
    if(markedForTriangleFlip){
        pitch1 *= -1.0f;
        pitch2 *= -1.0f;
    }
}

void TwoJointBone::MoveToTarget(FVector target, MMatrix &world, float deltatime){
    MoveToTarget(target);
    buildForward(world, deltatime);

    //debug:
    if(bLogEnabled){
        FVector reached = EndEffectorLocation() - StartEffector().getTranslation();

        //EndEffectorRelativeLocation();
        DebugHelper::logMessage("debugKinematic forward kinematic targeted: ", target);
        DebugHelper::logMessage("debugKinematic forward kinematic reached: ", reached);
        DebugHelper::logMessage("debugKinematic forward kinematic -----");
    }
}

/// @brief forward kinematics method default
/// @param target 
void TwoJointBone::MoveToTarget(FVector &target){
    resetRotations();

    //clamp to movement range
    clampTarget(target);

    //globale rotation
    float debugYaw = 0.0f;
    float debugPitch = 0.0;
    FVector2D x(1.0f, 0.0f);
    FVector2D z(0.0f, -1.0f);//DOWN-Z
    MMatrix lookAtTarget = MMatrix::createRotatorFrom(target, x, z, true, debugYaw, debugPitch);

    if(false){
        DebugHelper::logMessage(FString::Printf(
            TEXT("MMatrixRotationMaker yaw(%.2f), pitch(%.2f)"),
            MMatrix::radToDegree(debugYaw),
            MMatrix::radToDegree(debugPitch))
        );
    }
    


    //r1 = lookAtTarget;

    if(bLogEnabled){

        //debug look vector
        FVector boneAxisDefaultDown(0, 0, -1);
        boneAxisDefaultDown = lookAtTarget * boneAxisDefaultDown;
        boneAxisDefaultDown = boneAxisDefaultDown.GetSafeNormal() * target.Size();
        DebugHelper::logMessage("debugKinematic forward kinematic look result: ", boneAxisDefaultDown);
    }

    //pitch anziehen des beins / arms
    float pitchHip = 0.0f;
    float pitchKnee = 0.0f;

    float distance = target.Size();
    TwoBoneGeometricSolve::createPitchAnglesFor(distance, pitchHip, pitchKnee, t1, t2);
    flipTriangleIfMarkedWanted(pitchHip, pitchKnee); //further testing needed!


    //erst anziehen
    r1.pitchRadAdd(pitchHip);
    r2.pitchRadAdd(pitchKnee);

    //dann global drehen
    r1 = lookAtTarget * r1; //<-- lese richtung --
}



//new with move direction to use roll or yaw
void TwoJointBone::MoveToTarget(
    FVector target,
    MMatrix &world, 
    float deltatime,
    FVector &localForward
){
    MoveToTarget(target, localForward);
    buildForward(world, deltatime);

    //debug:
    if(bLogEnabled){
        FVector reached = EndEffectorLocation() - StartEffector().getTranslation();

        //EndEffectorRelativeLocation();
        DebugHelper::logMessage("debugKinematic forward kinematic (usingRoll) targeted: ", target);
        DebugHelper::logMessage("debugKinematic forward kinematic (usingRoll) reached: ", reached);
        DebugHelper::logMessage("debugKinematic forward kinematic -----");
    }
}


/// @brief Method for using roll instead of yaw rotation based on moving direction
/// @param target 
void TwoJointBone::MoveToTarget(FVector &target, FVector &localForward){
    resetRotations();

    //clamp to movement range
    clampTarget(target);

    //globale rotation
    FVector2D x(1.0f, 0.0f);
    FVector2D z(0.0f, -1.0f);//DOWN-Z
    MMatrix lookAtTarget = MMatrix::createRotatorFrom(target, x, z, true, localForward);

   
    if(bLogEnabled){
        //debug look vector
        FVector boneAxisDefaultDown(0, 0, -1);
        boneAxisDefaultDown = lookAtTarget * boneAxisDefaultDown;
        boneAxisDefaultDown = boneAxisDefaultDown.GetSafeNormal() * target.Size();
        DebugHelper::logMessage("debugKinematic forward kinematic (usingRoll) look result: ", boneAxisDefaultDown);
    }

    //pitch anziehen des beins / arms
    float pitchHip = 0.0f;
    float pitchKnee = 0.0f;

    float distance = target.Size();
    TwoBoneGeometricSolve::createPitchAnglesFor(distance, pitchHip, pitchKnee, t1, t2);
    flipTriangleIfMarkedWanted(pitchHip, pitchKnee); //further testing needed!


    //erst anziehen
    r1.pitchRadAdd(pitchHip);
    r2.pitchRadAdd(pitchKnee);

    //dann global drehen
    r1 = lookAtTarget * r1; //<-- lese richtung --
}













void TwoJointBone::MoveToTargetInverse(FVector target, float deltatime){
    MoveToTargetInverse(target);

    //CAUTION VERY IMPORTANT
    //it is very important to clear the end effectors own rotation,
    //otherwise the trajectory is not correctly followed by the hip(remove any pitch)
    removeRotationFromEndEffector();

    buildBackward(endEffectorWorld, deltatime); //builded from end effector
    
    //might be needed for rebuild.
    copyRotationFromInverseMatrices();

    //debug:
    if(bLogEnabled){
        FVector reached = EndEffectorRelativeLocation() * -1.0f;
        DebugHelper::logMessage("debugKinematic backward kinematic targeted: ", target);
        DebugHelper::logMessage("debugKinematic backward kinematic reached: ", reached);
        DebugHelper::logMessage("debugKinematic backward kinematic -----");
    }

    //debug draw
    if(bDrawBackwardReach){
        FVector start = endEffectorWorld.getTranslation();
        DebugHelper::showLineBetween(
            worldPtr,
            start,
            start + target,
            FColor::Yellow
        );
    }
    
}

/// @brief default inverse kinematics method
/// @param target 
void TwoJointBone::MoveToTargetInverse(FVector &target){
    resetRotations();
    //clamp to movement range
    clampTarget(target); //ok kein fehler

    //globale rotation
    FVector2D x(1.0f, 0.0f); //kleiner test
    FVector2D z(0.0f, 1.0f); //UP-Z
    MMatrix lookAtTarget = MMatrix::createRotatorFrom(target, x, z, true);
    //r2Inv = lookAtTarget; //r2 instead of r1

    //test look at matrix
    //es funktioniert einwandfrei!
    if(bLogEnabled){
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

    r2Inv = lookAtTarget * r2Inv;
    //r2Inv = lookAtTarget; // r2 instead of r1
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

    //new actor visibility
    applyTransformToActors(world, j1World, j2World);
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


    //debug rebuild forward
    //FVector target = EndEffectorRelativeLocation();
    //MoveToTarget(target, startEffectorWorld, deltatime);
    //buildForwardFromInverseMatrices(deltatime);
}

void TwoJointBone::copyRotationFromInverseMatrices(){
    r1 = r2Inv.transposedRotation();
    r2 = r1Inv.transposedRotation();
}

void TwoJointBone::draw(MMatrix &world, MMatrix &a, MMatrix &b, MMatrix &c, float dt){
    FVector wT = world.getTranslation();
    FVector aT = a.getTranslation();
    FVector bT = b.getTranslation();
    FVector cT = c.getTranslation();

    float frame = 0.01f;
    dt = std::max(frame * 3.0f, dt);

    //dt = 1.0f;
    FColor red = FColor::Red;
    FColor blue = FColor::Blue;
    FColor cyan = FColor::Cyan;

    if(colorFlagChanged){
        red = FColor::Green;
        blue = FColor::Yellow;
        cyan = FColor::Black;
    }
    if(bDrawToWorldStart){
        DebugHelper::showLineBetween(worldPtr, FVector(0,0,0), wT, FColor::Green, dt);
    }
    
    DebugHelper::showLineBetween(worldPtr, aT, wT, red, dt);
    DebugHelper::showLineBetween(worldPtr, aT, bT, blue, dt);
    DebugHelper::showLineBetween(worldPtr, bT, cT, cyan, dt);
}


MMatrix TwoJointBone::StartEffector(){
    return startEffectorWorld;
}

/// @brief returns the world end effector from the latest build inclding all rotation.
/// @return 
MMatrix TwoJointBone::EndEffector(){
    return endEffectorWorld;
}

/// @brief world location of end effector, rename!
/// @return 
FVector TwoJointBone::EndEffectorLocation(){
    return endEffectorWorld.getTranslation();
}

MMatrix TwoJointBone::EndEffectorTranslation(){
    FVector location = EndEffectorLocation();
    return MMatrix(location);
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





/// ----- LAYERED IK SECTION -----

//experimental!
MMatrix TwoJointBone::inverseTransform(){
    //M = RKnee * Tlower * Rhip * Tupper
    //M^-1 = Tupper^-1 * Rhip^-1 * Tlower^-1 * RKnee^-1
    //M^-1 = (Tupper^-1 * Rhip^-1) * Tlower^-1 * RKnee^-1
    //M^-1 = ((Tupper^-1 * Rhip^-1) * Tlower^-1) * RKnee^-1
    //M^-1 = (f * Tlower^-1) * RKnee^-1
    //M^-1 = (g) * RKnee^-1

    MMatrix r1InverseLocal = r1.transposedRotation();
    MMatrix f = t1Inv * r1InverseLocal;
    MMatrix g = f * t2Inv;

    MMatrix r2InverseLocal = r2.transposedRotation();
    MMatrix m1 = g * r2InverseLocal;
    return m1;
}


void TwoJointBone::overrideR2KneeRotation(MMatrix &rin){
    r2 = rin;
}


void TwoJointBone::useOtherColorType(){
    colorFlagChanged = true;
}




// --- api for actor attachment ---
void TwoJointBone::createLimbsIfNeeded(UWorld *world, float aHeight, float bHeight){
    if(world && autoCreateLimbs){
        int width = 10;
        AActor *top = CubeLimbMaker::createLimbPivotAtTop(width, width, aHeight, world);
        AActor *bottom = CubeLimbMaker::createLimbPivotAtTop(width, width, bHeight, world);
        attachLimbs(top, bottom);
    }
}


void TwoJointBone::attachLimbs(AActor *top, AActor *bottom){
    topActor = top;
    bottomActor = bottom;
}

void TwoJointBone::applyTransformToActors(MMatrix &world, MMatrix &top, MMatrix &bottom){
    if(autoCreateLimbs){ //has limbs.
        applyTransform(topActor, world.getTranslation(), top);
        applyTransform(bottomActor, top.getTranslation(), bottom);
    }
    
}

void TwoJointBone::applyTransform(AActor *ptr, FVector location, MMatrix &rotationMatrix){
    if(ptr){

        //limb umdrehen weil falsch.
        MMatrix rotatorPure = rotationMatrix.extarctRotatorMatrix();
        //rotatorPure = rotatorPure.transposedRotation();
        FRotator rotation = rotatorPure.extractRotator();

        ptr->SetActorLocation(location);
        ptr->SetActorRotation(rotation);
    }
}


void TwoJointBone::getActors(TArray<AActor *> &outArray){
    if(topActor){
        outArray.Add(topActor);
    }
    if(bottomActor){
        outArray.Add(bottomActor);
    }
}