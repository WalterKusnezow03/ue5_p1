#include "BoneAttachment.h"

#include "DebugPlugin/DebugHelper.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/liftOffFrame/SlipLiftOffFrameFinder.h"
#include "IkHumanoidModell/SharedRaycastParams/SharedRaycastParamManager.h"
#include "IkHumanoidModell/Ik/Controller/HipController/AnimationTime/AnimationTime.h"


BoneAttachment::BoneAttachment(){
    resetLocalForwardMovingDirection(); 
}

BoneAttachment::~BoneAttachment(){

}

void BoneAttachment::setWorld(UWorld *worldIn){
    world = worldIn;
}



void BoneAttachment::setupBone(FTwoLimbHipProperty &hipLimb){
    /*setWorld(hipLimb.GetWorld());
    bone.setup(hipLimb);
    setAsLeg();
    SetupPluckerJoints(hipLimb.GetOffset(), hipLimb.GetWorld());
    setInnerOffsetTranslation(hipLimb.GetOffset());

    defaultExtendedTranslationBottomToUp = FVector(
        0,
        0,
        hipLimb.GetFirstAndSecondSize()
    );*/
    setupBoneBase(hipLimb, hipLimb.GetOffset());
    setAsLeg();
}

void BoneAttachment::setupBoneBase(FTwoLimbProperty &package, FVector offset){
    setWorld(package.GetWorld());
    bone.setup(package);
    SetupPluckerJoints(offset, package.GetWorld());
    setInnerOffsetTranslation(offset);

    defaultExtendedTranslationBottomToUp = FVector(
        0,
        0,
        package.GetFirstAndSecondSize()
    );
}



void BoneAttachment::setAsLeg(){
    bone.markTriangleFlipAsWantedForLegs();
}

FVector BoneAttachment::defaultExtendedEndToStartLocal(){
    return defaultExtendedTranslationBottomToUp;
}

FVector BoneAttachment::endEffectorWorldLocation(){
    return bone.EndEffectorLocation();
}

/*
/// @brief setup bone
/// @param a part a (upper) lenght
/// @param b part b (lower) length
/// @param worldIn world for drawing
/// @param offset inner offset of bone attachment (hip offset for example)
void BoneAttachment::setupBone(
    float a, 
    float b, 
    UWorld *worldIn, 
    FVector offset
){
    bone.setup(a, b, worldIn);
    setWorld(worldIn);

    setInnerOffsetTranslation(offset);

    defaultExtendedTranslationBottomToUp = FVector(
        0,
        0,
        (std::abs(a) + std::abs(b))
    );

}

/// @brief setup bone
/// @param a part a (upper) lenght
/// @param b part b (lower) length
/// @param worldIn world for drawing
/// @param offset inner offset of bone attachment (hip offset for example)
/// @param massOfParent mass of parent body to pre calculate slip data 
/// @param defaultMotionTime default motion time of the stance phase
void BoneAttachment::setupBone(
    float a, 
    float b, 
    UWorld *worldIn, 
    FVector offset,
    float massOfParent,
    float defaultMotionTime
){
    setupBone(
        a, 
        b, 
        worldIn, 
        offset
    );

}

//player controller api width bone hand controller
void BoneAttachment::setupBone(float a, float b, UWorld *worldIn, FVector offset, float widthBone){
    bone.setup(a, b, worldIn, widthBone);
    setWorld(worldIn);

    setInnerOffsetTranslation(offset);

    defaultExtendedTranslationBottomToUp = FVector(
        0,
        0,
        (std::abs(a) + std::abs(b))
    );
}*/


//set inner offset translation (attachment displacement of bone)
//sets local and inverse direction.
void BoneAttachment::setInnerOffsetTranslation(FVector offset){
    innerOffset.setTranslation(offset);
    
    FVector offset1 = offset * -1.0f;
    innerOffsetInverse.setTranslation(offset1);

    p1.SetBoneTranslationDirection(offset);
    p1Invert.SetBoneTranslationDirection(offset1);
}

/// @brief transforms the root, adds inner offset to matrix
/// @param worldRoot 
/// @return 
MMatrix BoneAttachment::startEffectorTransformWorld(MMatrix &worldRoot){
    MMatrix transformStartEffector = worldRoot * innerOffset; //<-- lese richtung
    return transformStartEffector;
}

//to be ticked from outside
void BoneAttachment::setForwardTargetWorld(
    FVector &targetWorld,
    MMatrix &rootTranslation, 
    MMatrix &rootOrientation
){
    forwardTargetWorld = targetWorld;
    FVector forwardTargetLocal = inLocalSpace(
        targetWorld,
        rootTranslation,
        rootOrientation
    );
    setForwardTargetLocal(forwardTargetLocal);
}

void BoneAttachment::setForwardTargetLocal(FVector &target){
    forwardTarget = target;
}

void BoneAttachment::setBackwardTargetLocal(FVector &target){
    backwardTarget = target;
}

/// @brief designed to decide whether to use roll or yaw rotation based on moving direction
/// @param localMove 
void BoneAttachment::updateLocalForwardMovingDirection(FVector &localMove){
    localMovingDirectionSaved = localMove;
}

void BoneAttachment::resetLocalForwardMovingDirection(){
    localMovingDirectionSaved = FVector(1, 0, 0); // default move dir local is x forward
}

/// --- tick section ---

void BoneAttachment::TickNone(MMatrix &worldRoot, float deltatime){
    MMatrix transformStartEffector = startEffectorTransformWorld(worldRoot);
    bone.TickBuildForward(transformStartEffector, deltatime);
}

void BoneAttachment::TickForwardKinematic(MMatrix &worldRoot, float deltatime){
    MMatrix transformStartEffector = startEffectorTransformWorld(worldRoot);
    //bone.MoveToTarget(forwardTarget, transformStartEffector, deltatime); //old yaw only start effector
    bone.MoveToTarget(forwardTarget, transformStartEffector, deltatime, localMovingDirectionSaved);
}



// new layered kinematics - unklar wie das gesteuert werden soll
void BoneAttachment::TickForwardKinematicOutOfReachTarget(
    MMatrix &translation,
    MMatrix &orientation, //könnte temporäre kopie sein
    float deltatime
){
    //is out of reach:
    FVector outOfReach = bone.outOfreachDistance(forwardTarget);
    float minDistance = 1.0f;
    if (outOfReach.Size() > minDistance){
        //weil x nach vorne zeigt muss
        //der inner offset so gedreht werden dann er die 
        //höhe x erreicht

        //theta = asin(g/h) mit g: x of target
        //h: width of hip

        float g = outOfReach.X;
        float h = innerOffset.getTranslation().Size();

        float theta = std::asinf(g/h);
        float sign = outOfReach.Y < 0.0f ? 1.0f : -1.0f;

        float angleRad = sign * theta;

        orientation.yawRadAdd(angleRad);

        /**
         * es ist noch nicht klar ob der ansatz reicht!
         * mit hoher wahrscheinlichkeit nein!
         */
        //roll oder pitch der hip nicht beachtet. Achtung.

        DebugHelper::showScreenMessage("target out of reach ", outOfReach);
    }

    MMatrix worldRoot = translation * orientation;
    TickForwardKinematic(worldRoot, deltatime);
}

/// @brief 
/// @param transformRoot to be manipulated from movement!
/// @param orientationRoot orientation of root to updatee the position
/// @param deltatime 
void BoneAttachment::TickBackwardKinematic(
    MMatrix &translationRootToUpdate, 
    MMatrix &orientationRoot,
    float deltatime
){ 

    //new here:
    ApplyGravityToEndEffectorIfNotGrounded(deltatime);

    //target scheint bei 180 gespiegelt zu sein
    //was dafür spricht das es manuell gedreht werden muss:
    FVector targetcopy = orientationRoot * backwardTarget;
    bone.MoveToTargetInverse(targetcopy, deltatime);

    //works on 180
    FVector StartEffectorLocation = bone.StartEffector().getTranslation();
    MMatrix innerOffsetR = orientationRoot * innerOffset;
    FVector asVector(1, 0, 0);
    asVector = innerOffsetR * asVector;
    FVector rootPos = StartEffectorLocation - asVector;
    translationRootToUpdate.setTranslation(rootPos);

    /**
     * Sollte analog sein zu:
     * - asVector 
     * -1 * asVector
     * innerOffsetR * -1 * asVector
     * (orientationRoot * innerOffset) * -1 * asVector
     * (orientationRoot * innerOffset)^-1 * asVector
     * innerOffset^-1 * orientationRoot^-1 * asVector
     */
}

//fixes a lot of problems, move end effector to ground when backwards kinematic is applied.
void BoneAttachment::ApplyGravityToEndEffectorIfNotGrounded(float deltatime){

    FVector worldEndEffector = bone.EndEffectorLocation();
    if(worldEndEffector.Z > forwardTargetWorld.Z){
        //v(t) = v0 + at
        velocityEndEffectorOnBackwardsKinematic.Z += -981.0f * deltatime;

        //override
        //x(t) = x0 + v0t + 0.5at^2
        worldEndEffector += velocityEndEffectorOnBackwardsKinematic * deltatime;
        bone.OverrideEndEffectorWorldLocation(worldEndEffector);
    }
    else
    {
        velocityEndEffectorOnBackwardsKinematic = FVector(0, 0, 0);
    }
}

void BoneAttachment::TickKeepEndInWorldPlace(
    MMatrix &translationRoot, 
    MMatrix &orientationRoot,
    float deltatime
){
    FVector endEffectorWorld = bone.EndEffectorLocation();
    FVector endEffectorLocal = inLocalSpace(endEffectorWorld, translationRoot, orientationRoot);

    MMatrix worldRoot = translationRoot * orientationRoot; // M = T * R <-- lese richtung --
    MMatrix transformStartEffector = startEffectorTransformWorld(worldRoot);
    
    //bone.MoveToTarget(endEffectorLocal, transformStartEffector, deltatime);
    //new using actor orientation local
    bone.MoveToTarget(endEffectorLocal, transformStartEffector, deltatime, localMovingDirectionSaved);

}





//projection of trajectories
FVector BoneAttachment::inLocalSpace(
    FVector &worldFrame, 
    MMatrix &rootTranslation, 
    MMatrix &rootOrientation
){

    //M = Troot * Rhip * Tinner <-- lese richtung --
    //M^-1 = Tinner^-1 * Rhip^-1 * Troot^-1 <-- lese richtung --
    MMatrix rootTranslationInv = rootTranslation.invertedTranslation();
    MMatrix rotationInv = rootOrientation.transposedRotation();

    MMatrix transformInverse = rotationInv * rootTranslationInv;
    MMatrix M = innerOffsetInverse * transformInverse;

    FVector local = M * worldFrame;
    return local;
}

FVector BoneAttachment::inWorldSpace(
    FVector &localFrame, 
    MMatrix &rootTranslation, 
    MMatrix &rootOrientation
){
    //M_start = TRoot * R * Tinner <-- lese richtung
    MMatrix transform = rootTranslation * rootOrientation;
    MMatrix M = transform * innerOffset;

    FVector worldFrame = M * localFrame;
    return worldFrame;
}

FVector BoneAttachment::hipRelativeLocationToEndEffector(
    MMatrix &rootTranslation,
    MMatrix &rootOrientation
){  
    /*
    //M_start = TRoot * R * Tinner <-- lese richtung
    MMatrix a = rootOrientation * innerOffset;
    MMatrix M = rootTranslation * a;

    FVector hipStart = M.getTranslation();
    FVector footEnd = bone.EndEffectorLocation();

    FVector relative = hipStart - footEnd; // AB = B - A //endStart = Start - end
    */

    FVector footEndWorld = bone.EndEffectorLocation();
    FVector localEnd = inLocalSpace(
        footEndWorld,
        rootTranslation,
        rootOrientation
    );
    FVector relative = localEnd * -1.0f;
    DebugHelper::logMessage("boneAttachment: foot to hip A: ", relative);

    //wrong bei backward kinematic 180 degree rotation!
    if(false){
        relative = bone.EndEffectorRelativeLocation() * -1.0f;
        DebugHelper::logMessage("boneAttachment: foot to hip B: ", relative);
    }
    

    return relative;
}




/**
 * slip force
 */
/// ----- new dynamic slipforce -------
FVector BoneAttachment::StaticSlipVelocity(
    FVector &lookDir,
    float velocityDown, 
    float mass,
    float deltatime,
    bool isInStance
){

    FVector EndEffectorLocalRotationSpace = bone.EndEffectorRelativeLocation();

    FVector velocity = container.StaticSlipVelocity(
        lookDir,
        EndEffectorLocalRotationSpace,
        EndEffectorIsGrounded(),
        velocityDown,
        mass,
        deltatime,
        isInStance
    );

    //weighted by distance from trajectory
    float maxDistanceFromTrajectory = 30.0f;
    float weight = VerticalDistanceFromTrajectoryAsScalar(maxDistanceFromTrajectory);
    velocity *= weight;
    //DebugHelper::showScreenMessage(FString::Printf(TEXT("Velocity Weighted %.2f"), weight), FColor::Red);
    return velocity;
}
/// ----- new dynamic slipforce end -------


 // ------ DEPRECATED precalculated slip force: deprecated --------
/// @brief will return the slip data, based if has reached target or not 
/// @param mass 
/// @return 
SlipContainer &BoneAttachment::slipData(MMatrix &orientation){
    
    //if reached target: setup
    FVector endEffectorTrajectory = bone.EndEffectorRelativeLocation();

    //move to WORLD rotation!
    endEffectorTrajectory = orientation * endEffectorTrajectory;

    container.setup(
        bone.lengthOfBone(),
        endEffectorTrajectory
    );
    

    return container;
}
// ------ DEPRECATED precalculated slip force: deprecated end --------





// ------ DEPRECATED precalculated slip force: deprecated --------
void BoneAttachment::setupSlipDataOnStanceBegin(
    MMatrix &orientation,
    FVector &defaultForwardFrame,
    float time,
    float velocityDown,
    float mass
){
    FVector aWorld = bone.EndEffectorRelativeLocation();
    MMatrix r1 = orientation.transposedRotation();
    FVector aLocal = r1 * aWorld;


    FVector moveDir(1, 0, 0);
    moveDir = orientation * moveDir;



    //so lassen
    bone.clampTarget(aLocal);

    FVector liftOffDefault = defaultForwardFrame;
    FVector liftOffHacked = defaultForwardFrame;
    liftOffHacked.X = aLocal.X * -1.0f; //hack

    FVector bLocal = liftOffHacked; 

    //MOVE TO WORLD LIFT OFF
    FVector bWorld = orientation * bLocal;

    container.setupInterpolatedD(
        aWorld, //local in rotated world space
        bWorld, //local in rotated world space
        moveDir,
        time,
        velocityDown,
        mass
    );
}

/// -- new method with regards to next foot step. Will calculate the lift off frame dynamically--
/// ----> testing needed!
void BoneAttachment::setupSlipDataOnStanceBegin(
    MMatrix &orientation,
    MMatrix &translation,
    FVector &otherLegWorldSpace,
    FVector &nextTrajectoryOfOtherLegWorldSpace, //next projceted frame of next leg target, !!velocity removed!!
    float time,
    float velocityDown,
    float velocityHorizontal,
    float mass,
    FVector &defaultForwardFrameFallback
){
    float heelOffEpsilon = 0.0f; //might be changed.
    
    //both trajectories for heel off in local space.
    FVector currentReached = bone.EndEffectorRelativeLocation(); //no rotation removed, start to end effector vector.
    MMatrix r1 = orientation.transposedRotation();
    FVector currentLocalSpace = r1 * currentReached;

    float B1 = time;
    float B2 = B1; //einfacher
    float F1 = AnimationTime::AnimationTimeBasedOnHorizontalAndVerticalVelocity(
        otherLegWorldSpace, nextTrajectoryOfOtherLegWorldSpace, velocityHorizontal, velocityDown
    );

    time = B1 + F1 + B2;
    DebugHelper::logMessage(
        FString::Printf(TEXT("time total B1 F1 B2 %.2f"), 
            time
        )
    );




    //lift off frame relative to start effector found, from (0,0,0), rotation space supported.
    //uses 2 world coordinates as input, returns a local lift off trajectory, which is not derotated.
    FVector targetWorld = bone.EndEffectorLocation();
    SlipLiftOffFrameFinder liftOffFinder;
    FVector liftOffFrame = liftOffFinder.FindLiftOffFrameRelativeLocalToNextHipFromWorldTrajectories(
        targetWorld, // A0 (current foot target / "A_1" in your notes), WORLD SPACE
        nextTrajectoryOfOtherLegWorldSpace, // B0 (next foot target), WORLD SPACE
        bone.lengthOfBone(),
        heelOffEpsilon
    );


    //must be given by bone controller.
    FVector moveDir(1, 0, 0);
    moveDir = orientation * moveDir;

    //debug
    if(!LiftOffTrajectoryIsValid(liftOffFrame, moveDir)){
        DebugHelper::logMessage("LiftOffTrajectory not valid");
        /*setupSlipDataOnStanceBegin(
            orientation,
            defaultForwardFrameFallback,
            time,
            velocityDown,
            mass
        );*/
        return;
    }

    //move both trajectories to world rotation space and setup slip data
    FVector startLocalWorldRotationSpace = currentReached; //is already in world Rotation.
    FVector liftoffLocalWorldRotationSpace = liftOffFrame;


    

    //setup
    /*
    container.setupInterpolatedD(
        startLocalWorldRotationSpace, //local in rotated world space
        liftoffLocalWorldRotationSpace, //local in rotated world space
        moveDir,
        time,
        velocityDown,
        mass
    );*/
    container.setupInterpolatedD(
        startLocalWorldRotationSpace, //local in rotated world space
        liftoffLocalWorldRotationSpace, //local in rotated world space
        moveDir,
        B1,
        F1,
        B2,
        velocityDown,
        mass
    );

}




bool BoneAttachment::LiftOffTrajectoryIsValid(
    FVector &liftOffFrameLocal,
    FVector &moveDir
){
    FVector down(0.0f, 0.0f, -1.0f);
    FVector directionOfLiftOff = liftOffFrameLocal.GetSafeNormal();
    float minDot = 0.95f;
    float angle = FVector::DotProduct(directionOfLiftOff, down);
    if (angle >= minDot){
        FString message = TEXT("SlipLiftOffFrameFinder setupSlipDataOnStanceBegin");
        message += TEXT("Fallback to fake trajectory, angle too narrow!");

        message += FString::Printf(TEXT("%.2f"), MMatrix::radToDegree(angle));
        DebugHelper::showScreenMessage(message);
        DebugHelper::logMessage(message);
        return false;
    }
    FVector up = -1.0f * down;
    if(FVector::DotProduct(directionOfLiftOff, up) > 0.0f){
        return false;
    }
    if(FVector::DotProduct(directionOfLiftOff, moveDir) >= 0.0f){
        return false;
    }
    



    FString message = FString::Printf(
        TEXT("SlipLiftOffFrameFinder setupSlipDataOnStanceBegin angle: %.2f"), 
        angle
    );
    DebugHelper::showScreenMessage(message);
    DebugHelper::logMessage(message);
    return true;
}
// ------ DEPRECATED precalculated slip force: deprecated end --------





/**
 * targte info
 */
bool BoneAttachment::reachedTarget(){
    return distanceFromTarget() <= 2.0f; // 10.0f;
}

bool BoneAttachment::reachedTargtZ(){
    FVector endeffector = bone.EndEffectorLocation();
    float distZ = forwardTargetWorld.Z - endeffector.Z;

    return distZ <= 2.0f; // 10.0f;
}


float BoneAttachment::distanceFromTarget(){
    FVector endeffector = bone.EndEffectorLocation();
    return FVector::Dist(endeffector, forwardTargetWorld);
}


/**
 * get actors for damaged owner setup
 */
void BoneAttachment::getActors(TArray<AActor *> &outArray){
    bone.getActors(outArray);
}





// ---- reset -----
void BoneAttachment::ResetAndRebuild(
    MMatrix &translation,
    MMatrix &orientation
){
    //M = T * R * Tinner
    MMatrix TR = translation * orientation;
    MMatrix M = TR * innerOffset;
    bone.ResetAndRebuild(M);
}





void BoneAttachment::UpdateGroundTruth(FVector &ground){
    worldGroundTruth = ground;
}



float BoneAttachment::VerticalDistanceFromTrajectoryAsScalar(float maxDistance){
    FVector location = endEffectorWorldLocation();
    float heightDistance = location.Z - worldGroundTruth.Z;
    if(heightDistance > 0.0f){
        float scalar = heightDistance / maxDistance;


        //---- TESTING NEEDED ----
        //one over x hier einbauen --> zu stark
        //scalar = FMath::Clamp(scalar, 0.000000001f, 1.0f);
        //scalar = 1.0f / scalar;

        //FMath::Clamp(Value, MinValue, MaxValue);
        scalar = 1.0f - FMath::Clamp(scalar, 0.0f, 1.0f);

        return scalar;
    }
    return 1.0f;
}

bool BoneAttachment::EndEffectorIsGrounded(){
    //10 cm
    FVector location = endEffectorWorldLocation();
    if(location.Z - worldGroundTruth.Z  <= 10.0f){
        return true;
    }

    if (FVector::DistSquared(location, worldGroundTruth) < float(10 * 10))
    {
        if(world){
            DebugHelper::showLineBetween(
                world,
                location,
                location + FVector(0, 0, 20),
                FColor::Purple
            );
        }
        return true;
    }
    return false;
}






//// ---- PLUECKER JOINTS ----
void BoneAttachment::SetupPluckerJoints(FVector &offset, UWorld *worldIn){
    bone.SetParentInterface(this);
    FVector offset1 = offset * -1.0f;

    p1 = Joint(offset, worldIn);
    p1Invert = Joint(offset1, worldIn);
}

void BoneAttachment::UpstreamPropagate(
    FJointKinematicPropagatePackage &package
){

    // ---- TODO ----

    if(HasParentInterface()){
        parentInterface->UpstreamPropagate(package);
    }
}

void BoneAttachment::DownstreamPropagate(
    FJointKinematicPropagatePackage &package
){
    MMatrix resultWorld = p1.TickAndBuildThisJoint(package);
    package.transform = resultWorld;

    bone.DownstreamPropagate(package);
}