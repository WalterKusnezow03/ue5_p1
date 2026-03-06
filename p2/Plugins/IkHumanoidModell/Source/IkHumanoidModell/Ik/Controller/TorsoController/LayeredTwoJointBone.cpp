#include "LayeredTwoJointBone.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/FHumanoidControllerSetupPackage.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FTwoLimbProperty.h"

LayeredTwoJointBone::LayeredTwoJointBone(){
    world = nullptr;
    handIsSetup = false;
}
LayeredTwoJointBone::~LayeredTwoJointBone(){
    world = nullptr;
}

// ---- SETUP SECTION ----

void LayeredTwoJointBone::setup(
    EArmType typeIn,
    FHumanoidControllerSetupPackage &package
){
    armTypeSaved = typeIn;
    world = package.GetWorld();

    FTwoLimbProperty &torsoProperty = package.GetTorsoSize();
    torsoBone.setup(torsoProperty);
    torsoBone.useOtherColorType();

    //NEW
    torsoBone.markTriangleFlipAsWantedForLegs();
    
    
    FTwoLimbProperty &armProperty = package.GetArmSize();
    armBone.setup(armProperty);
    armBone.SetConstraint(ETwoBoneConstraint::EFlipDown); //bend direction of arm.

    //new
    torsoBone.AddChildToLowerJoint(armBone.GetTopJoint());





    findDefaultLocalTorsoTarget(
        torsoProperty.GetSizeFirst(), 
        torsoProperty.GetSizeSecond()
    );
    findTotalMotionCircleSize(
        torsoProperty.GetSizeFirst(), 
        torsoProperty.GetSizeSecond(),
        armProperty.GetSizeFirst(), 
        armProperty.GetSizeSecond()
    );

    SetupHand(typeIn, package);
    
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


void LayeredTwoJointBone::findDefaultLocalTorsoTarget(
    float torsoHeight, 
    float shoulderWidth
){
    //find shoulder target moved to upper direction overall. Bone must move up.
    float sign = armTypeSaved == EArmType::ELeft ? -1.0f : 1.0f;
    FVector up(0, 0, std::abs(torsoHeight));
    FVector side(0, std::abs(shoulderWidth) * sign, 0);
    localTorsoEndEffectorTarget = up + side;
}

//DEPRECATED!


void LayeredTwoJointBone::SetupHand(
    EArmType typeIn,
    FHumanoidControllerSetupPackage &package
){
    if(!handIsSetup && package.HandsMarkedWanted()){
        handIsSetup = true;
        hand.setup(package, typeIn);
    }
}





// ---- TICK SECTION ----

// abstract tick will find whether to follow a target or not
void LayeredTwoJointBone::Tick(
    MMatrix &actorTranslation,
    MMatrix &actorRotation, 
    float deltatime
){
    if(collapseEnabledFlag){
        TickPhysicsCollpase(actorTranslation, actorRotation, deltatime);
        return;
    }

    //follow actor
    if(itemIsAttached()){
        worldTarget = HandTargetWorldBasedOnAttachedItem();

        //looks ok
        if(true && bDrawDebugLines){
            DebugHelper::showLineBetween(
                world,
                actorTranslation.getTranslation(),
                worldTarget,
                FColor::Purple,
                0.1f
            );
        }
        

        //DebugHelper::logMessage("item world target: ", worldTarget);
        TickForwardKinematicsWorldTarget(
            actorTranslation,
            actorRotation,
            deltatime
        );
        TickHandController(actorRotation, deltatime);
        return;
    }

    //else do nothing
    TickBuildNone(
        actorTranslation,
        actorRotation,
        deltatime
    );
    TickHandController(actorRotation, deltatime);
}

/// @brief special tick for the world target reach
/// @param actorTranslation 
/// @param actorRotation 
/// @param deltatime 
void LayeredTwoJointBone::TickForwardKinematicsWorldTarget(
    MMatrix &actorTranslation,
    MMatrix &actorRotation,
    float deltatime
){
    //target in both bone reach:
    //build torso default
    //fk arm
    FVector localTargetInArmSystem = moveToLocalSpaceOfArm(
        actorTranslation, 
        actorRotation, 
        worldTarget
    );

    //EXPERIMENTS: - KORREKT
    //ACHTUNG: Wenn der arm immer von einer reinen TRanslation aus gebaut wird, 
    //Und X wieder default mässig nach vorne schaut, 
    //muss der arm durch anpassug des targets nochmal künstlich gedreht werden
    //wie bei rotierten bein targets!
    localTargetInArmSystem = actorRotation * localTargetInArmSystem;


    if (localTargetInArmMotionCircle(localTargetInArmSystem))
    {

        MMatrix actorTransform = actorTranslation * actorRotation; // M = T * R <--lese richtung --
        buildTorsoBoneNone(actorTransform, deltatime);
        MMatrix shoulderStartM = torsoBone.EndEffectorTranslation();
        armBone.MoveToTarget(localTargetInArmSystem, shoulderStartM, deltatime);
        UpdateAttachedActorLowerArmDirection();

        //DebugHelper::logMessage("In Local Space Arm! ");
    }
    else
    {
        //target out of bone reach:
        //clamp target world to motion circle from actor
        //find closest point shoulder in full length,
        //override end effector hand,
        //arm ik to shoulder pos
        //fk to shoulder pos for torso bone

        //might not be needed
        worldTarget = clampToFullMotionCircleWorld(
            actorTranslation,
            worldTarget
        );

        //DebugHelper::logMessage("Not! In Local Space Arm! ");


        //might work INCORRECT
        FVector shoulderTargetLocal = shoulderLocalTargetFromTargetArmIfOutOfRange(
            worldTarget,            // world target
            localTargetInArmSystem, // local arm target
            actorTranslation,
            actorRotation
        );

        bool drawShoulderTarget = true;
        if(drawShoulderTarget && bDrawDebugLines){
            FVector worldstart = actorTranslation.getTranslation();
            FVector localRotatedSpace = actorRotation * shoulderTargetLocal;
            DebugHelper::showLineBetween(
                world,
                worldstart,
                worldstart + localRotatedSpace,
                FColor::Purple,
                0.1f
            );
        }

        bool drawHandtarget = true;
        if(drawHandtarget && bDrawDebugLines){
            FVector actorStart = actorTranslation.getTranslation();
            FVector localRotatedSpace = actorRotation * shoulderTargetLocal;
            FVector worldstart = actorStart + localRotatedSpace;

            DebugHelper::showLineBetween(
                world,
                worldstart,
                worldstart + localTargetInArmSystem,
                FColor::Orange,
                0.1f);
        }



        //move shoulder to target
        MMatrix actorTransform = actorTranslation * actorRotation; // M = T * R <--lese richtung --
        torsoBone.MoveToTarget(shoulderTargetLocal, actorTransform, deltatime);


        // ------ DAS HIER STIMMT NICHT WENN ACTOR GEDREHT IST, ARM NICHT EINROTIERT ------
        // ------ UNKLAR: IST DAS LOCAL TARGET RICHTIG ROTIERT?
        //build to shoulder , build arm from raw world translation
        MMatrix shoulderStartM = torsoBone.EndEffectorTranslation();

        //build from shoulder LOCATION (?) to target hand local
        armBone.MoveToTarget(localTargetInArmSystem, shoulderStartM, deltatime);
        UpdateAttachedActorLowerArmDirection();
    }
}

/// @brief default build: torso bone with default alignment and 90 degree side rotation
/// @param actorTransform 
/// @param deltatime 
void LayeredTwoJointBone::buildTorsoBoneNone(
    MMatrix &actorTransform, 
    float deltatime
){
    //overrideTorsoBreastRotationDefault();
    //torsoBone.TickBuildForward(actorTransform, deltatime);

    //move to default target
    torsoBone.MoveToTarget(localTorsoEndEffectorTarget, actorTransform, deltatime);
    
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
    float deltatime
){
    //build torso
    MMatrix actorTransform = actorTranslation * actorRotation;
    buildTorsoBoneNone(actorTransform, deltatime);

    //build arm from shoulder end without rotation
    MMatrix shoulderStartM = torsoBone.EndEffectorTranslation();
    armBone.TickBuildForward(shoulderStartM, deltatime);
    UpdateAttachedActorLowerArmDirection();
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
    FVector &worldTargetIn
){
    //move to local space once default torso setup
    //actor = T * R
    //actorInverse = R^-1 * T^-1
    MMatrix actorR1 = actorOrientation.transposedRotation();
    MMatrix actorT1 = actorTranslation.invertedTranslation();


    MMatrix torsoDefault(localTorsoEndEffectorTarget);
    MMatrix torsoInv = torsoDefault.invertedTranslation();

    //M = T * R * torso
    //M^-1 = torso^-1 * R^-1 * T^-1
    MMatrix A = torsoInv * actorR1;
    MMatrix localSpaceMatrix = A * actorT1;

    FVector localSpaceTarget = localSpaceMatrix * worldTargetIn;


    //looks correct all the time ??
    bool drawLocal = true;
    if(drawLocal && bDrawDebugLines){
        FVector worldstart = actorTranslation.getTranslation();
        FVector localRotatedSpace = actorOrientation * localSpaceTarget;
        DebugHelper::showLineBetween(
            world,
            worldstart,
            worldstart + localRotatedSpace,
            FColor::Black,
            0.1f
        );
    }








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
    MMatrix &actorOrientation
){
    FVector directionFromArm = armBone.lengthOfBone() * targetLocalArm.GetSafeNormal();
    FVector shoulderWorldtarget = targetWorld - directionFromArm;

    //M = T * R
    //M^-1 = R^-1 * T^-1
    MMatrix actorR1 = actorOrientation.transposedRotation();
    MMatrix actorT1 = actorTranslation.invertedTranslation();
    
    //MMatrix torsoR1 = torsoRotation.transposedRotation();
    //MMatrix rotationInverse = torsoR1 * actorR1;
    //MMatrix actorInverse = rotationInverse * actorT1;

    MMatrix actorInverse = actorR1 * actorT1;

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
    attachedItemInterface = actor;
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

/// actor attachment update bone direction
/// for hand carried items
void LayeredTwoJointBone::UpdateAttachedActorLowerArmDirection(){
    if(itemIsAttached()){

        
        //IIKCarryInterface::UpdateLowerArm(EArmType typeArm, const FVector &direction)
        attachedItemInterface->UpdateLowerArm(
            armTypeSaved, 
            armBone.DirectionOfMiddleToEndEffector()
        );
    }

    
}






//
// --- api for get actors:apply damaged owner casted mesh actor ---
void LayeredTwoJointBone::getActors(TArray<AActor *> &outArray){
    torsoBone.getActors(outArray);
    armBone.getActors(outArray);

    //add hand here!
    //if(handIsSetup){}
    hand.getActors(outArray);
}


void LayeredTwoJointBone::UpdateHandOnCarriedItem(){
    if(attachedItemInterface){
        hand.Update(attachedItemInterface);
    }
}

void LayeredTwoJointBone::TickHandController(MMatrix &playerOrientation, float deltatime){
    if(handIsSetup){
        //update finger targets
        UpdateHandOnCarriedItem();

        MMatrix T = armBone.EndEffectorTranslation(); //world.
        //M = T * R <--lese richtung --
        MMatrix M = T * playerOrientation;
        hand.Tick(M, deltatime);
    }
}




/// ---- pluecker joints ----


void LayeredTwoJointBone::ReactToDamage(const FCustomHitResult &hitResult){
    torsoBone.ReactToDamage(hitResult);
    armBone.ReactToDamage(hitResult);
}

void LayeredTwoJointBone::TickPhysicsCollpase(
    MMatrix &actorTranslation,
    MMatrix &actorRotation,
    float deltatime
){//none
    /*
    MMatrix M = actorTranslation * actorRotation; // T * R <--

    
    if(collapseEnabledFlag){
        torsoBone.Tick(deltatime, M);
    }*/
}

void LayeredTwoJointBone::SetStateCollapse(bool flag){
    IJointInterface::SetStateCollapse(flag);
    torsoBone.SetStateCollapse(flag);
    armBone.SetStateCollapse(flag);
}
