#include "BoneAttachment.h"


BoneAttachment::BoneAttachment(){

}

BoneAttachment::~BoneAttachment(){

}

FVector BoneAttachment::defaultExtendedEndToStartLocal(){
    return extendedTranslationBottomToUp;
}

FVector BoneAttachment::endEffectorWorldLocation(){
    return bone.EndEffectorLocation();
}

void BoneAttachment::setupBone(float a, float b, UWorld *world, FVector offset){
    bone.setup(a, b, world);

    innerOffset.setTranslation(offset);

    offset *= -1.0f;
    innerOffsetInverse.setTranslation(offset);

    extendedTranslationBottomToUp = FVector(
        0,
        0,
        (std::abs(a) + std::abs(b))
    );
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



/// --- tick section ---

void BoneAttachment::TickNone(MMatrix &worldRoot, float deltatime){
    MMatrix transformStartEffector = startEffectorTransformWorld(worldRoot);
    bone.TickBuildForward(transformStartEffector, deltatime);
}

void BoneAttachment::TickForwardKinematic(MMatrix &worldRoot, float deltatime){
    MMatrix transformStartEffector = startEffectorTransformWorld(worldRoot);
    bone.MoveToTarget(forwardTarget, transformStartEffector, deltatime);
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
    bone.MoveToTargetInverse(backwardTarget, deltatime);

    //update root
    //M = offset * startEffector <---lese richtung--- (noch ohne rotation)

    //M = R_hip * T_innerOffset <-- im regelfall
    //M^-1 = T_innerOffset^-1 * R_hip^-1
    MMatrix rotationInverse = orientationRoot.transposedRotation();
    MMatrix innerOffsetInverseRotated = innerOffsetInverse * rotationInverse;

    //letzen schritt zur welt wandern
    MMatrix StartEffector = bone.StartEffector();
    MMatrix updatedTransform = innerOffsetInverseRotated * StartEffector;
    FVector locationUpdate = updatedTransform.getTranslation();
    translationRootToUpdate.setTranslation(locationUpdate);

}


void BoneAttachment::TickKeepEndInWorldPlace(
    MMatrix &translationRoot, 
    MMatrix &orientationRoot,
    float deltatime
){
    /**
     * NOCH VERBUGGT!
     */

    FVector endEffectorWorld = bone.EndEffectorLocation();
    DebugHelper::showScreenMessage("in world space keep end: ", endEffectorWorld); //verändert sich, unklar wieso!
    FVector endEffectorLocal = inLocalSpace(endEffectorWorld, translationRoot, orientationRoot);
    DebugHelper::showScreenMessage("in local space keep end: ", endEffectorLocal);


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
    //M^-1 = Tinner^-1 * Rhip^-1 * Troot^-1
    MMatrix translationInv = rootTranslation.invertedTranslation();
    MMatrix rotationInv = rootOrientation.transposedRotation();

    MMatrix transformInverse = rotationInv * translationInv;
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
    DebugHelper::logMessage("boenAttachment: foot to hip A: ", relative);

    relative = bone.EndEffectorRelativeLocation() * -1.0f;
    DebugHelper::logMessage("boenAttachment: foot to hip B: ", relative);

    return relative;
}




/**
 * slip force
 */

/// @brief will return the slip data, based if has reached target or not
/// @param mass 
/// @return 
SlipContainer BoneAttachment::slipData(){
    SlipContainer container;
    //if reached target: setup
    FVector reached = bone.EndEffectorRelativeLocation();
    if(reached.Size() <= forwardTarget.Size()){
        container.setup(
            bone.lengthOfBone(),
            reached
        );
    }

    return container;
}