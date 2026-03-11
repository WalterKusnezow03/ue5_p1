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



void SpatialTransformRoot::applyJointConstraint(FVector &w, FVector &v){
    
    //heright velocity never locked.

    //DEBUG!
    float scaleDebug = 0.0001f; // 0.0f;
    scaleDebug = 0.9f;

    v.X *= scaleDebug;
    v.Y *= scaleDebug;
}

void SpatialTransformRoot::OnForwardPlueckerFinished(){
    //override translation with plücker integrated position, since we have 6DOF and 
    //need to save the new world update!
    //unlike the bone where its right now NOT done. (might be implemented later, unclear)
    translation = resultTranslation;
    OnForwardPlueckerFinishedCutTranslationAgainstGround();

    //DebugHelper::showScreenMessage("SpatialTransformRoot::Result ", translation.ToString());
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
    groundNormal = FVector(0, 0, 0);
    if (IsGrounded(current)){
        contactFloor = true;
    }
    UpdateGroundConstraint(current);

    if(bLogEnabled){
        DebugHelper::showScreenMessage("SpatialTransformRoot::", groundContactConstraint.ToString(), FColor::Red);
    }
    
}





void SpatialTransformRoot::SafeWorldResultCache(
    const MMatrix &prevTransform, 
    const MMatrix &rootMatrix
){
    //start of root.
    worldLocationCache = rootMatrix.getTranslation();
    worldTransformCache = rootMatrix; //root prev is alwyas identity.
    worldRotatorCache = rootMatrix.extractRotator();
    worldRotationCache.setRotation(worldRotatorCache);
}

/// override delta plücker implementation!

void SpatialTransformRoot::forwardDeltaPluecker(
    FVector &angularVelocity, //w
    FVector &linearVelocity,  //v
    Matrix3x3 &outDeltaRotation,
    FVector &outDeltaTranslation,
    float deltatime
){
    
    //das default
    
    /*
    X = |R        0_3x3|
        |R*s(w)   R    |
    
        R in SO3 Gruppe
    

    j_v(w,v) = X * i_v(w,v)

    */

    //erst j_v ausrechnen
    /*
        |w
        |v
    -------
    a b |aw + bv  //jetzt mal w1 genannt
    c d |cw + dv  //v1 genannt
    */
    Matrix3x3 a = RotationSO3;
    Matrix3x3 b;
    b.makeZero();

    Matrix3x3 c1 = RotationSO3;
    Matrix3x3 c2 = Matrix3x3::skew(axis());
    Matrix3x3 c = c1 * c2;

    Matrix3x3 d = RotationSO3;

    FVector w1 = a * angularVelocity + b * linearVelocity;
    FVector v1 = c * angularVelocity + d * linearVelocity;
    FVector w1_constrained = w1;
    FVector v1_constrained = v1;






    //FVector w1_constrained = angularVelocity;
    //FVector v1_constrained = linearVelocity;


    //add own velocities for integration of position
    spatialVelocity.AddOwnVelocityTo(w1_constrained, v1_constrained); //in current space add current space velocity




    //resultierende velocities für den joint constrainen.
    applyConstraints(w1_constrained, v1_constrained); 


    

    
    //----> TRANSLATION NOT FOR ROOT!

    
    //dann velocity integrieren
    //deltaTwist(w1, v1)
    //Matrix3x3 outDeltaRotation; -> is updated inside convertPlueckerToSE3components
    //FVector outDeltaTranslation; -> is updated inside convertPlueckerToSE3components 
    Matrix3x3::convertPlueckerToSE3components(
        w1_constrained, v1_constrained, outDeltaRotation, outDeltaTranslation, deltatime
    );


    //CUTSOM TRANSLATION INTEGRATION FOR ROOT!
    //root instant default integration
    outDeltaTranslation = v1_constrained * deltatime;

    // ---- TESTING NEEDED ! ----

    //UNKLAR OB HIER CONSTRAINTED ODER NICHT!
    angularVelocity = w1_constrained;
    linearVelocity = v1_constrained;



    velocitycache = v1_constrained;


}