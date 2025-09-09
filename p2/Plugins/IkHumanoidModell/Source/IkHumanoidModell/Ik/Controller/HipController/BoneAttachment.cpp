#include "BoneAttachment.h"

#include "GameCore/DebugHelper.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/liftOffFrame/SlipLiftOffFrameFinder.h"


BoneAttachment::BoneAttachment(){
    resetLocalForwardMovingDirection(); 
}

BoneAttachment::~BoneAttachment(){

}

void BoneAttachment::setWorld(UWorld *worldIn){
    world = worldIn;
}

//not tested!
void BoneAttachment::setAsLeg(){
    bone.markTriangleFlipAsWantedForLegs();
}

FVector BoneAttachment::defaultExtendedEndToStartLocal(){
    return defaultExtendedTranslationBottomToUp;
}

FVector BoneAttachment::endEffectorWorldLocation(){
    return bone.EndEffectorLocation();
}

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

    innerOffset.setTranslation(offset);

    offset *= -1.0f;
    innerOffsetInverse.setTranslation(offset);

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

    //debug
    //return;

    container.initDefaultForParameterD(
        bone.lengthOfBone(),
        massOfParent,
        defaultMotionTime
    );
}


//default D NEW
/*
void BoneAttachment::setupBone(
    float a, 
    float b, 
    UWorld *worldIn, 
    FVector offset,
    float defaultMotionTime,
    MMatrix &orientation,
    FVector &defaultForward, //forward frame local
    float time,
    float mass
){
    setupBone(
        a, 
        b, 
        worldIn, 
        offset
    );
    float velocityDown = 0.0f;
}*/

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

//flipped for falling
void BoneAttachment::TickKeepEndInWorldPlaceNegHeightTrajectory(
    MMatrix &translationRoot, 
    MMatrix &orientationRoot,
    float deltatime
){
    FVector endEffectorWorld = bone.EndEffectorLocation();
    FVector endEffectorLocal = inLocalSpace(endEffectorWorld, translationRoot, orientationRoot);
    
    //slip leg down
    if(endEffectorLocal.Z > 0.0f){
        endEffectorLocal *= -1.0f;
    }

    MMatrix worldRoot = translationRoot * orientationRoot; // M = T * R <-- lese richtung --
    MMatrix transformStartEffector = startEffectorTransformWorld(worldRoot);
    bone.MoveToTarget(endEffectorLocal, transformStartEffector, deltatime);
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


//new!
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



    /*
    ONLY FORWARD WALK

    weil das forward target auch projeziert wird, 
    und somit nicht das standard forward target ist,
    könnte es sein dass das einfach gedreht werden muss
    beim setup,
    bzw die x länge übernommen und diese gespiegelt, nicht die höhe
    was wie ein hack wirkt könnte funktionieren!

    JA! Funktioniert!

    ----TODO:----
    Könnte auch durch forward target ersetzt werden, wäre eingeschlossener!!
    */
    FVector liftOffDefault = defaultForwardFrame;
    FVector liftOffHacked = defaultForwardFrame;
    liftOffHacked.X = aLocal.X * -1.0f; //hack
    FVector bLocal = liftOffHacked; 


    /**
     * Der lift off frame ist nach der rotation wieder entrotiert,
     * die Yaw rotation muss entfernt werden und der frame nach hinten gespiegelt werden
     

    MMatrix removeRotation = orientation.transposedRotation();
    FVector aLocalDeRotated = removeRotation * aLocal;
    bLocal.X = aLocalDeRotated.X * -1.0f; //hack spiegel
    */


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

/// -- new method with regards to next foot step. --
/// ----> testing needed!
void BoneAttachment::setupSlipDataOnStanceBegin(
    MMatrix &orientation,
    MMatrix &translation,
    FVector &nextTrajectoryOfOtherLegWorldSpace, //next projceted frame of next leg target, !!velocity removed!!
    float time,
    float velocityDown,
    float mass
){
    
    //both trajectories for heel off in local space.
    FVector currentReached = bone.EndEffectorRelativeLocation(); //no rotation removed, start to end effector vector.
    MMatrix r1 = orientation.transposedRotation();
    FVector currentLocalSpace = r1 * currentReached;

    float heelOffEpsilon = 1.0f; //might be changed.




    FVector targetWorld = bone.EndEffectorLocation();
    SlipLiftOffFrameFinder liftOffFinder;
    FVector liftOffFrame = liftOffFinder.FindLiftOffFrameRelativeLocalToNextHipFromWorldTrajectoriesNew(
        targetWorld, // A0 (current foot target / "A_1" in your notes), WORLD SPACE
        nextTrajectoryOfOtherLegWorldSpace, // B0 (next foot target), WORLD SPACE
        bone.lengthOfBone(),
        heelOffEpsilon
    );

    bool debugLiftOffFrame = true;
    if(debugLiftOffFrame){
        //log
        FVector localSpace = r1 * liftOffFrame; //aLocalRotation = R^-1 * aWorld
        FString msg = TEXT("Lift off frame: ");
        msg += liftOffFrame.ToString();
        DebugHelper::logMessage("SlipLiftOffFrameFinder: BoneAttachment: ",msg);

        //draw
        /*
        FVector worldFrame = translation * liftOffFrame;
        DebugHelper::showLineBetween(
            worldPointer,

        )*/
    }

    //move both trajectories to world rotation space and setup slip data
    FVector startLocalWorldRotationSpace = currentReached; //is already in world Rotation.
    FVector liftoffLocalWorldRotationSpace = liftOffFrame;
    // orientation * liftOffFrameLocalNoRotation;

    //must be given by bone controller.
    FVector moveDir(1, 0, 0);
    moveDir = orientation * moveDir;

    //setup
    container.setupInterpolatedD(
        startLocalWorldRotationSpace, //local in rotated world space
        liftoffLocalWorldRotationSpace, //local in rotated world space
        moveDir,
        time,
        velocityDown,
        mass
    );


}














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