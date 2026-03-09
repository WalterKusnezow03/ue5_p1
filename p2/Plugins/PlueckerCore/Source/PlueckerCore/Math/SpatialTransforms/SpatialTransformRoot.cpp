#include "SpatialTransformRoot.h"




//local transform of root with: M = T * R
MMatrix SpatialTransformRoot::Transform(){
    //M = T * R <-- lese richtung
    MMatrix t = Translation();
    MMatrix r = Rotation();
    return t * r;
}

MMatrix SpatialTransformRoot::TransformInverse(){
    //M = R^-1 * T^-1 <-- lese richtung
    MMatrix t1 = TranslationInverted();
    MMatrix r1 = RotationTransposed(); //R^T = R^-1
    return r1 * t1;
}


//DEBUG!
void SpatialTransformRoot::applyJointConstraint(FVector &w, FVector &v){
    v.X = 0.0f;
    v.Y = 0.0f; 
}

void SpatialTransformRoot::OnForwardPlueckerFinished(){
    //override translation with plücker integrated position, since we have 6DOF and 
    //need to save the new world update!
    //unlike the bone where its right now NOT done. (might be implemented later, unclear)
    translation = resultTranslation;
    OnForwardPlueckerFinishedCutTranslationAgainstGround();

    DebugHelper::showScreenMessage("SpatialTransformRoot::Result ", translation.ToString());
}

void SpatialTransformRoot::OnForwardPlueckerFinishedCutTranslationAgainstGround(){
    //dont allow going below ground. 
    //from latest raycast: groundTruth
    
    translation.Z = std::max(groundTruth.Z, translation.Z);
}








FVector &SpatialTransformRoot::axis(){
    return none; //no axis in root joint.
}

//ONLY SECOND MATRIX RESULT IS OF INTEREST, prev is always at (0,0,0),
//because we build from the world origin here!
void SpatialTransformRoot::UpdateFloorContact(const MMatrix &prev, const MMatrix &current){
    
    contactFloor = false;
    if (IsGrounded(current)){
        contactFloor = true;
    }
    UpdateGroundConstraint(current);

    DebugHelper::showScreenMessage("SpatialTransformRoot::", groundContactConstraint.ToString(), FColor::Red);
}





void SpatialTransformRoot::SafeWorldResultCache(
    const MMatrix &prevTransform, 
    const MMatrix &rootMatrix
){
    //start of root.
    worldLocationCache = rootMatrix.getTranslation();
    worldTransformCache = rootMatrix; //root prev is alwyas identity.
    worldRotatorCache = rootMatrix.extractRotator();
}