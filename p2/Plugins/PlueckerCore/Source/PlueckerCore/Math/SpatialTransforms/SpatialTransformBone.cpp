#include "SpatialTransformBone.h"



//local transform of bone with: M = R * T <--lese richtung--
MMatrix SpatialTransformBone::Transform(){
    //M = R * T <-- lese richtung
    MMatrix t = Translation();
    MMatrix r = Rotation();
    return r * t;
}

//local transform inverse of bone with: M = T^-1 * R^-1 <--lese richtung--
MMatrix SpatialTransformBone::TransformInverse(){
    //M = T^-1 * R^-1 <-- lese richtung
    MMatrix t1 = TranslationInverted();
    MMatrix r1 = RotationTransposed(); //R^T = R^-1
    return t1 * r1;
}


//if prev or current joint is grounded: contact true!
void SpatialTransformBone::UpdateFloorContact(const MMatrix &prev, const MMatrix &current){
    
    contactFloor = false;
    //if (IsGrounded(prev) && IsGrounded(current))
    if (IsGrounded(prev) || IsGrounded(current)){
        contactFloor = true;
    }
    UpdateGroundConstraint(current);
}



// cache world transform matrix for attached actors and
// world Center of mass calculations
void SpatialTransformBone::SafeWorldResultCache(
    const MMatrix &prevTransform, 
    const MMatrix &endBone
){
    //start of bone.
    worldLocationCache = prevTransform.getTranslation();
    worldRotatorCache = endBone.extractRotator(); //accumulated world rotation for actor. Is correct, dont change.

    //world matrix as result for a actor or com world calculation: 
    //M = T * R
    worldTransformCache.makeIdentity();
    worldTransformCache.setRotation(worldRotatorCache);
    worldTransformCache.setTranslation(worldLocationCache);
    worldRotationCache.setRotation(worldRotatorCache);
}