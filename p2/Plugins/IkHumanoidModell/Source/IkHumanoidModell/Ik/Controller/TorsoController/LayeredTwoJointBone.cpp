#include "LayeredTwoJointBone.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"


LayeredTwoJointBone::LayeredTwoJointBone(){
    world = nullptr;
}
LayeredTwoJointBone::~LayeredTwoJointBone(){
    world = nullptr;
}

// ---- SETUP SECTION ----

void LayeredTwoJointBone::setup(
    EArmType typeIn,
    float hipBreast,
    float breastShoulder,
    float upperArm,
    float lowerArm,
    UWorld *worldIn
){
    armTypeSaved = typeIn;
    world = worldIn;
    torsoBone.setup(hipBreast, breastShoulder, world);
    torsoBone.useOtherColorType();
    armBone.setup(upperArm, lowerArm, world);
    findTotalMotionCircleSize(hipBreast, breastShoulder, upperArm, lowerArm);
}

void LayeredTwoJointBone::findTotalMotionCircleSize(
    float hipTorso, 
    float torsoShoulder, 
    float armUpper, 
    float armLower
){
    totalMotionCircleSize =
        std::abs(torsoShoulder) +
        std::abs(hipTorso) +
        std::abs(armUpper) +
        std::abs(armLower);
}

// ---- TICK SECTION ----

// abstract tick will find whether to follow a target or not
void LayeredTwoJointBone::Tick(
    MMatrix &actorTranslation,
    MMatrix &actorRotation, 
    MMatrix &torsoRotation,
    float deltatime
){
    //follow actor
    if(itemIsAttached()){
        worldTarget = HandTargetWorldBasedOnAttachedItem();
        TickForwardKinematicsWorldTarget(
            actorTranslation,
            actorRotation,
            torsoRotation,
            deltatime
        );
        return;
    }

    //else do nothing
    TickBuildNone(
        actorTranslation,
        actorRotation,
        torsoRotation,
        deltatime
    );
}

/// @brief special tick for the world target reach
/// @param actorTranslation 
/// @param actorRotation 
/// @param deltatime 
void LayeredTwoJointBone::TickForwardKinematicsWorldTarget(
    MMatrix &actorTranslation,
    MMatrix &actorRotation, 
    MMatrix &torsoRotation,
    float deltatime
){
    //target in both bone reach:
    //build torso default
    //fk arm
    FVector localTargetInArmSystem = moveToLocalSpaceOfArm(
        actorTranslation, 
        actorRotation, 
        torsoRotation,
        worldTarget
    );
    
    if(localTargetInArmMotionCircle(localTargetInArmSystem)){

        MMatrix combinedRotation = actorRotation * torsoRotation;
        MMatrix actorTransform = actorTranslation * combinedRotation; // M = T * R <--lese richtung --
        buildTorsoBoneNone(actorTransform, deltatime);
        MMatrix shoulderStartM = torsoBone.EndEffector();
        armBone.MoveToTarget(localTargetInArmSystem, shoulderStartM, deltatime);
    }else{
        //target out of bone reach:
        //clamp target world to motion circle from actor
        //find closest point shoulder in full length,
        //override end effector hand,
        //arm ik to shoulder pos
        //fk to shoulder pos for torso bone
        FVector worldTargetClamped = clampToFullMotionCircleWorld(
            actorTranslation,
            worldTarget
        );


        FVector shoulderTargetLocal = shoulderLocalTargetFromTargetArmIfOutOfRange(
            worldTarget,            // world target
            localTargetInArmSystem, // local arm target
            actorTranslation,
            actorRotation,
            torsoRotation
        );

        //build to shoulder , build arm from raw world translation
        MMatrix shoulderStartM = torsoBone.EndEffectorTranslation();

        //build from shoulder LOCATION (?) to target hand local
        armBone.MoveToTarget(localTargetInArmSystem, shoulderStartM, deltatime);
    }
}

/// @brief default build: torso bone with default alignment and 90 degree side rotation
/// @param actorTransform 
/// @param deltatime 
void LayeredTwoJointBone::buildTorsoBoneNone(
    MMatrix &actorTransform, 
    float deltatime
){
    overrideTorsoBreastRotationDefault();
    torsoBone.TickBuildForward(actorTransform, deltatime);
}

/// @brief will roll the torso top to own direction (schlüsselbein artig nach links rechts)
void LayeredTwoJointBone::overrideTorsoBreastRotationDefault(){
    float sign = armTypeSaved == EArmType::ELeft ? -1.0f : 1.0f;
    float roll = (M_PI / 2.0f) * sign; //90 degree rotation to left if left arm torso
    MMatrix rollMat;
    rollMat.rollRadAdd(roll);
    torsoBone.overrideR2KneeRotation(rollMat);
}

/// @brief will build the both torso and arm bones with no fk or ik
void LayeredTwoJointBone::TickBuildNone(
    MMatrix &actorTranslation,
    MMatrix &actorRotation, 
    MMatrix &torsoRotation,
    float deltatime
){
    //build torso
    MMatrix rotationCombined = actorRotation * torsoRotation;
    MMatrix actorTransform = actorTranslation * rotationCombined;
    buildTorsoBoneNone(actorTransform, deltatime);

    //build arm from shoulder end without rotation
    MMatrix shoulderStartM = torsoBone.EndEffectorTranslation();
    armBone.TickBuildForward(shoulderStartM, deltatime);
}

/// @brief maybe needed to be updated every frame, local target is followed (?)
/// @param target 
void LayeredTwoJointBone::setWorldTarget(FVector &target){
    worldTarget = target;
}

void LayeredTwoJointBone::setLocalTarget(FVector &target){
    localTarget = target;
}

/// @brief start effector of hip, first start effector rotation
/// @return 
MMatrix LayeredTwoJointBone::startEffectorRotation(){
    MMatrix none; //TODO IMPLEMENT
    return none;
}

/// @brief moves target into local shoulder space
/// @param actorTranslation 
/// @param actorOrientation 
/// @param worldTarget 
/// @return 
FVector LayeredTwoJointBone::moveToLocalSpaceOfArm(
    MMatrix &actorTranslation,
    MMatrix &actorOrientation, 
    MMatrix &torsoRotation,
    FVector &worldTargetIn
){
    // --- CAUTION ISSUE MIGHT ARISE ---
    //The orientation of the hip is rotated by 180 dergree to build the torso
    //might be replaced by inverted bone instead and internal rotation change
    //for z axis,
    //OR use End Effector Location as pure translational Matrix
    //which moves a target into the shoulder system
    bool useRawTranslation = true;

    // ------ TODO ------
    //hier könnte es sinnvoll sein den torso bone
    //build None zu machen damit ein das koordinaten system direkt default 
    //torso setup ist und die inverse korrekt ist.
    MMatrix combinedRotation = actorOrientation * torsoRotation;
    MMatrix actorTransformTorso = actorTranslation * combinedRotation;
    buildTorsoBoneNone(
        actorTransformTorso,
        0.0f // delta time irrelevant here
    );

    //move to local space once default torso setup
    //actor = T * R
    //actorInverse = R^-1 * T^-1
    MMatrix actorR1 = actorOrientation.transposedRotation();
    MMatrix actorT1 = actorTranslation.invertedTranslation();
    MMatrix actorInv = actorR1 * actorT1;

    //localSpace = actorInverse * Bone1Inverse
    MMatrix torsoInv;
    if(useRawTranslation){
        //Just T^-1 because its more simple than using the whole inverse matrix if
        //rot of player is modified.
        torsoInv = torsoBone.EndEffectorTranslation().invertedTranslation();
    }
    else
    {
        torsoInv = torsoBone.inverseTransform();
    }

    MMatrix localSpaceMatrix = actorInv * torsoInv;

    FVector localSpaceTarget = localSpaceMatrix * worldTargetIn;
    return localSpaceTarget;
}




bool LayeredTwoJointBone::localTargetInMotionCircle(FVector &target){
    return target.Size() <= totalMotionCircleSize;
}

bool LayeredTwoJointBone::localTargetInArmMotionCircle(FVector &target){
    return armBone.targetIsInMotionCircle(target);
}

/// @brief clamps a world target to the full range circle in world system
/// @param actorTranslation 
/// @param targetWorld 
FVector LayeredTwoJointBone::clampToFullMotionCircleWorld(
    MMatrix &actorTranslation,
    FVector &targetWorld
){
    FVector actorLocation = actorTranslation.getTranslation();
    FVector connect = targetWorld - actorLocation; // AB = B - A
    FVector worldCopy = targetWorld;
    if (connect.Size() >= totalMotionCircleSize){
        connect = connect.GetSafeNormal();
        worldCopy = actorLocation + connect * totalMotionCircleSize; //gx = A + t(B-A)
    }
    return worldCopy;
}

/// @brief creates the shoulder LOCAL target to reach when local arm target was out of range
/// @param target 
FVector LayeredTwoJointBone::shoulderLocalTargetFromTargetArmIfOutOfRange(
    FVector &targetWorld,
    FVector &targetLocalArm,
    MMatrix &actorTranslation,
    MMatrix &actorOrientation,
    MMatrix &torsoRotation
){
    FVector directionFromArm = armBone.lengthOfBone() * targetLocalArm.GetSafeNormal();
    FVector shoulderWorldtarget = targetWorld - directionFromArm;

    //M = T * R
    //M^-1 = R^-1 * T^-1
    MMatrix actorR1 = actorOrientation.transposedRotation();
    MMatrix actorT1 = actorTranslation.invertedTranslation();
    MMatrix torsoR1 = torsoRotation.transposedRotation();

    MMatrix rotationInverse = torsoR1 * actorR1;
    MMatrix actorInverse = rotationInverse * actorT1;

    FVector shoulderTargetLocal = actorInverse * shoulderWorldtarget;
    return shoulderTargetLocal;
}




/// ---- actor attachment ----
void LayeredTwoJointBone::dropCarriedItem(){
    attachedItemInterface = nullptr;
}

/// @brief attaches actor if not nullptr
/// @param actor 
void LayeredTwoJointBone::attachOrReplaceCarriedItem(IIkCarryInterface *actor){
    if(actor){
        attachedItemInterface = actor;
    }
}

bool LayeredTwoJointBone::itemIsAttached(){
    return attachedItemInterface != nullptr;
}

FVector LayeredTwoJointBone::HandTargetWorldBasedOnAttachedItem(){
    if(itemIsAttached()){
        CarriedItemPositionData &positionData = attachedItemInterface->getItemPositionDataRef();
        FVector target =
            armTypeSaved == EArmType::ELeft ? 
            positionData.leftHandWorld() : 
            positionData.rightHandWorld();
        return target;
    }
    return FVector(0, 0, 0); //should be never returned if actorIsAttached is asked before
}