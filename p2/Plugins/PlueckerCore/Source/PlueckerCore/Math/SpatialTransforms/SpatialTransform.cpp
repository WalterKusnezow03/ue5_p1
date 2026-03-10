#include "SpatialTransform.h"


void SpatialTransform::SetWorld(UWorld *worldIn){
    world = worldIn;
}

SpatialTransform::SpatialTransform(){

}

SpatialTransform::~SpatialTransform(){

}

SpatialTransform::SpatialTransform(const SpatialTransform &other){
    if(this != &other){
        *this = other;
    }
}

SpatialTransform &SpatialTransform::operator=(const SpatialTransform &other){
    if(this != &other){
        constraint = other.constraint;
        RotationSO3 = other.RotationSO3;
        translation = other.translation;
        resultTranslation = other.resultTranslation;
        //world = other.world;
        SetWorld(other.world);
    }
    return *this;
}

void SpatialTransform::setTranslation(FVector &other){
    translation = other;
    resultTranslation = other;
}


void SpatialTransform::applyConstraints(FVector &w, FVector &v){
    //w Angular velocity
    //v Linear velocity
    applyJointConstraint(w, v);
    applyGravityConstraint(w, v);
}

void SpatialTransform::applyJointConstraint(FVector &w, FVector &v){
    constraint.ApplyRotationConstraint(w);
    constraint.ApplyPositionConstraint(v);
}

void SpatialTransform::applyGravityConstraint(FVector &w, FVector &v){
    groundContactConstraint.ApplyRotationConstraint(w);
    groundContactConstraint.ApplyPositionConstraint(v);
}





FJointConstraint &SpatialTransform::GetConstraint(){
    return constraint;
}

//makes forward pluecker and refreshes the given w and v
//transform update is saved, R and T SE3 are updated
void SpatialTransform::forwardPluecker(
    FVector &angularVelocity, //w
    FVector &linearVelocity,  //v
    float deltatime
){
    /*FString message = FString::Printf(
        TEXT("SpatialTransform::forwardPluecker(%s)(%s)"),
        *angularVelocity.ToString(),
        *linearVelocity.ToString()
    );
    DebugHelper::showScreenMessage(message, FColor::Yellow);*/

    
    Matrix3x3 outDeltaRotation;
    FVector outDeltaTranslation;
    forwardDeltaPluecker(
        angularVelocity, // w
        linearVelocity,  // v
        outDeltaRotation,
        outDeltaTranslation,
        deltatime
    );

    //Integrieren
    resultTranslation = outDeltaTranslation + translation; // sollte so ok sein
    RotationSO3 = RotationSO3 * outDeltaRotation; //<-- lese richtung so --




    // --- TODO: WARNING --- UNCLEAR ---
    //achtung: hier müssen noch constraints rein, 
    //es wurden KEINE JOINTS MIT velocity constraints getestet!

    //VERY IMPORTANT TO CALL!
    OnForwardPlueckerFinishedLogDeltaTranslation(outDeltaTranslation);
    OnForwardPlueckerFinished();
}

void SpatialTransform::forwardDeltaPluecker(
    FVector &angularVelocity, //w
    FVector &linearVelocity,  //v
    Matrix3x3 &outDeltaRotation,
    FVector &outDeltaTranslation,
    float deltatime
){
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

    /*FString message = FString::Printf(
        TEXT("SpatialTransform::forwardPluecker Result w(%s) v(%s)"),
        *w1.ToString(),
        *v1.ToString()
    );
    DebugHelper::showScreenMessage(message, FColor::Yellow);*/

    
    FVector w1_constrained = w1;
    FVector v1_constrained = v1;
    //resultierende velocities für den joint constrainen.
    applyConstraints(w1_constrained, v1_constrained); 

    /*FString message = FString::Printf(
        TEXT("SpatialTransform::forwardPlueckerConstrained w(%s) v(%s)"),
        *w1_constrained.ToString(),
        *v1_constrained.ToString()
    );
    DebugHelper::showScreenMessage(message, FColor::Yellow);*/




    //dann velocity integrieren
    //deltaTwist(w1, v1)
    //Matrix3x3 outDeltaRotation; -> is updated inside convertPlueckerToSE3components
    //FVector outDeltaTranslation; -> is updated inside convertPlueckerToSE3components 
    Matrix3x3::convertPlueckerToSE3components(
        w1_constrained, v1_constrained, outDeltaRotation, outDeltaTranslation, deltatime
    );





    // ---- TESTING NEEDED ! ----

    //UNKLAR OB HIER CONSTRAINTED ODER NICHT!
    angularVelocity = w1_constrained;
    linearVelocity = v1_constrained;



    velocitycache = v1_constrained;

    /*//refresh, for propagation to next joint
    angularVelocity = w1;
    linearVelocity = v1;*/

}



MMatrix SpatialTransform::operator*(const MMatrix &worldPrev){

    //MWorld = prev * R * T <--
    MMatrix transformLocal = Transform();
    MMatrix result = worldPrev * transformLocal; // lese richtung

    //save world result into cache
    SafeWorldResultCache(worldPrev, result);

    UpdateFloorContact(worldPrev, result);
    return result;
}


MMatrix SpatialTransform::Translation(){
    MMatrix translationMat(resultTranslation);
    return translationMat;
}

MMatrix SpatialTransform::Rotation(){
    MMatrix rotationLocal;
    CopyRotationTo(rotationLocal);
    return rotationLocal;
}

MMatrix SpatialTransform::TranslationInverted(){
    FVector t1 = resultTranslation * -1.0f;
    MMatrix translationMat1(t1);
    return translationMat1;
}

MMatrix SpatialTransform::RotationTransposed(){
    MMatrix r1 = Rotation();
    r1.transposeRotation();
    return r1;
}

/*
void SpatialTransform::UpdateFloorContact(const MMatrix &prev, const MMatrix &current){
    
    contactFloor = false;
    //if (IsGrounded(prev) && IsGrounded(current))
    if (IsGrounded(prev) || IsGrounded(current)){
        contactFloor = true;
    }
    UpdateGroundConstraint(current);
}*/

void SpatialTransform::UpdateGroundConstraint(const MMatrix &worldResult){
    UpdateGroundConstraintPitchAndPosition();
    UpdateGroundConstraintRoll(worldResult);
}

void SpatialTransform::UpdateGroundConstraintPitchAndPosition(){
    groundContactConstraint.allowPitchRotationPositive = true;
    groundContactConstraint.allowPitchRotationNegative = true;
    groundContactConstraint.isGroundedFlag = false;

    if(contactFloor){
        groundContactConstraint.allowPitchRotationNegative = false;
        groundContactConstraint.isGroundedFlag = true;
    }
}

// --- NOT TESTED ---
void SpatialTransform::UpdateGroundConstraintRoll(const MMatrix &worldResult){
    //roll constraint based on current bone rotation: R rotation!
    
    // --- NOT TESTED ---

    groundContactConstraint.allowRollRotationPositive = true;
    groundContactConstraint.allowRollRotationNegative = true;
    if(contactFloor){
        FVector zAxis(0, 0, 1);
        zAxis = worldResult * zAxis;

        //ALSO DOT NORMAL ? UNKLAR! ROTATION IN GROUND VERHINDERN!

        float heightValue = zAxis.Z;

        groundContactConstraint.allowRollRotationPositive = heightValue > 0.0f;
        groundContactConstraint.allowRollRotationNegative = heightValue < 0.0f;
    }



}





// --- external transform override ---

Matrix3x3 SpatialTransform::GetRotation() const {
    return RotationSO3;
}

void SpatialTransform::CopyRotationTo(MMatrix &copyInside) const {
    std::vector<float> values = RotationSO3.Copy();
    copyInside.setRotation(values);
}


void SpatialTransform::OverrideRotation(const MMatrix &rotationMatIn){
    std::vector<float> rotationValues = rotationMatIn.CopyRotation();
    RotationSO3.Override(rotationValues);
}


void SpatialTransform::OverrideRotation(const Matrix3x3 &rotationMatIn){
    RotationSO3 = rotationMatIn;
}

void SpatialTransform::OverrideRotation(const FRotator &r){
    RotationSO3.setRotation(r);
}












/// ----- backward force -----

//makes forward pluecker and refreshes the given w and v
//transform update is saved, R and T SE3 are updated
void SpatialTransform::backwardWrench(
    FVector &moment, //n
    FVector &force //f
){

    /*
    X^T = |R^T        s(t)R^T|
            |0_3x3      R^T    |



        R in SO3 Gruppe


    j_f(n,f) = X^T * i_f(n,f)

    */

    //erst j_v ausrechnen
    /*
        |n
        |f
    -------
    a b |aw + bv  //n1 genannt
    c d |cw + dv  //f1 genannt
    */
    Matrix3x3 a = RotationSO3;
    a.transpose();

    Matrix3x3 b1 = RotationSO3;
    b1.transpose();

    Matrix3x3 b2 = Matrix3x3::skew(axis());
    Matrix3x3 b = b2 * b1;

    Matrix3x3 c;
    c.makeZero();

    Matrix3x3 d = a;


    FVector n1 = a * moment + b * force;
    FVector f1 = c * moment + d * force;

    //constraints limitieren:
    /*FVector f1_constrained = f1;
    FVector n1_constrained = n1;
    applyConstraints(f1_constrained, n1_constrained); // ob hier noch unklar.*/

    moment = n1;
    force = f1;
}






//// ----- internal torque ------

FVector SpatialTransform::Force(float mass){
    FVector totalForce(0, 0, 0);
    totalForce += GravityForce(mass);
    totalForce += NormalForce(mass);
    //ShowVector(totalForce, 100.0f, FColor::Purple); //normal force is too low?
    return totalForce;
}

FVector SpatialTransform::GravityForce(float mass){
    if(true || !contactFloor){
        FVector g(0, 0, -981.0f);
        return g * mass;
    }
    return FVector(0, 0, 0);
}

#include "PlueckerCore/Math/SpatialTransforms/Force/NormalForce.h"
FVector SpatialTransform::NormalForce(float mass){
    return FNormalForce::NormalForce(groundNormal, groundPenetration, velocitycache);
}

FVector SpatialTransform::Torque(const FVector &force, const FVector &centerOfMass){
    //return Torque(RotationSO3, force, centerOfMass);
    return Torque(RotationSO3, force, centerOfMass);
}

FVector SpatialTransform::Torque(const Matrix3x3 &rotationSpace, const FVector &force, const FVector &centerOfMass){
    FVector comWorld = rotationSpace * centerOfMass;
    return FVector::CrossProduct(comWorld, force);
}


//// ----- GROUNDED CHECKS -----
void SpatialTransform::UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn){
    ignoreParams = ignoreParamsIn;
}

//debug
void SpatialTransform::ShowPosition(const MMatrix &other){
    ShowPosition(other.getTranslation());
}

void SpatialTransform::ShowPosition(FVector pos){
    DebugHelper::showLineBetween(
        world,
        pos,
        pos + FVector(10,0,100),
        FColor::Purple,
        1.0f
    );
}

void SpatialTransform::ShowVector(const FVector &vec, float size, FColor color){
    DebugHelper::showLineBetween(
        world,
        worldLocationCache,
        worldLocationCache + vec.GetSafeNormal() * size,
        color,
        0.1f
    );
}


//debug

bool SpatialTransform::IsGrounded(const MMatrix &prev){
    FVector t = prev.getTranslation();
    return IsGrounded(t);
}

bool SpatialTransform::IsGrounded(FVector &Start){
    if(Start.Z < staticGroundHeight){
        groundNormal = FVector(0, 0, 1);
        return true;
    }

    if(world){
        
        FVector dir(0, 0, -1.0f);

        float offsetUp = 200.0f;
        FVector StartRay = Start + FVector(0, 0, offsetUp);
        float sizeRay = 20.0f + offsetUp;

        FVector outputHit;
        FHitResult outHitResult;

        Raycaster raycaster;
        bool result = raycaster.performRaycast(
            world,
            StartRay,
            dir,
            ignoreParams,
            sizeRay,
            outputHit,
            outHitResult
        );
        groundTruth = outputHit;
        groundNormal = outHitResult.ImpactNormal.GetSafeNormal();
        SetGroundPenetration(result, Start, outputHit);
        
        
        
        if(false){
            DrawGroundPenetration(Start);
            if(result){
                FVector smallDir = groundNormal * 10.0f;
                DebugHelper::showLineBetween(world, Start, Start + smallDir, FColor::Cyan, 1.0f);
            }else{
                FVector smallDir(10,0,0);
                DebugHelper::showLineBetween(world, Start, Start + smallDir, FColor::Yellow, 1.0f);
            }
        }
        

        
        
        

        return result;
    }
    return false;
}


void SpatialTransform::SetGroundPenetration(bool result, const FVector &start, const FVector &hit){
    groundPenetration = 0.0f;

    float groundEpsilon = 10.0f; //extra for bone size
    if(result && BelowGround(start, hit, groundEpsilon)){ //start lower than hit, stuck in ground
        FVector groundPenetrationDirection = start - hit; //AB = B - A
        float dot = FVector::DotProduct(groundNormal, groundPenetrationDirection.GetSafeNormal());
        if(dot <= 0.0f){
            //is inside floor
            groundPenetration = groundPenetrationDirection.Size() * dot;
        }
    }
}

void SpatialTransform::DrawGroundPenetration(const FVector &pos){
    DebugHelper::showLineBetween(world, pos, pos + FVector(0, 0, groundPenetration), FColor::Orange, 1.0f);
}

bool SpatialTransform::BelowGround(const FVector &check, const FVector &ground){
    return check.Z < ground.Z;
}

bool SpatialTransform::BelowGround(const FVector &check, const FVector &ground, float epislon){
    return check.Z < (ground.Z + epislon);
}




FVector SpatialTransform::ActorTranslationFromCache() const {
    return worldLocationCache;
}

FRotator SpatialTransform::ActorRotationFromCache() const {
    return worldRotatorCache;
}


FVector SpatialTransform::centerOfMassWorld(const FVector &comLocal) const {
    return worldTransformCache * comLocal; //move to world space. (transform by bone or root managed properly.)
}