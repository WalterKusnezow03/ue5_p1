#include "Matrix6x6.h"

void Matrix6x6::SetWorld(UWorld *worldIn){
    world = worldIn;
}

Matrix6x6::Matrix6x6(){

}

Matrix6x6::~Matrix6x6(){

}

Matrix6x6::Matrix6x6(const Matrix6x6 &other){
    if(this != &other){
        *this = other;
    }
}

Matrix6x6 &Matrix6x6::operator=(const Matrix6x6 &other){
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

void Matrix6x6::setTranslation(FVector &other){
    translation = other;
    resultTranslation = other;
}


void Matrix6x6::applyConstraints(FVector &w, FVector &v){
    //w Angular velocity
    //v Linear velocity
    constraint.ApplyRotationConstraint(w);
    constraint.ApplyPositionConstraint(v);

    groundContactConstraint.ApplyRotationConstraint(w);
    groundContactConstraint.ApplyPositionConstraint(v);
}

void Matrix6x6::OverrideConstraint(FJointConstraint &in){
    constraint = in;
}

FJointConstraint &Matrix6x6::GetConstraint(){
    return constraint;
}

//makes forward pluecker and refreshes the given w and v
//transform update is saved, R and T SE3 are updated
void Matrix6x6::forwardPluecker(
    FVector &angularVelocity, //w
    FVector &linearVelocity,  //v
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

    //moved to tmp fuction
    /*
    Matrix3x3 a = RotationSO3;
    Matrix3x3 b;
    b.makeZero();

    Matrix3x3 c1 = RotationSO3;
    Matrix3x3 c2 = Matrix3x3::skew(translation);
    Matrix3x3 c = c1 * c2;

    Matrix3x3 d = RotationSO3;

    FVector w1 = a * angularVelocity + b * linearVelocity;
    FVector v1 = c * angularVelocity + d * linearVelocity;

    //constraints limitieren:
    FVector w1_constrained = w1;
    FVector v1_constrained = v1;
    applyConstraints(w1_constrained, v1_constrained); // ob hier noch unklar.

    //dann velocity integrieren
    //deltaTwist(w1, v1)
    Matrix3x3 outDeltaRotation;
    FVector outDeltaTranslation;
    Matrix3x3::convertPlueckerToSE3components(
        w1_constrained, v1_constrained, outDeltaRotation, outDeltaTranslation, deltatime
    );*/


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

}


void Matrix6x6::forwardDeltaPluecker(
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
   Matrix3x3 c2 = Matrix3x3::skew(translation);
   Matrix3x3 c = c1 * c2;

   Matrix3x3 d = RotationSO3;

   FVector w1 = a * angularVelocity + b * linearVelocity;
   FVector v1 = c * angularVelocity + d * linearVelocity;

   //constraints limitieren:
   FVector w1_constrained = w1;
   FVector v1_constrained = v1;
   applyConstraints(w1_constrained, v1_constrained); // ob hier noch unklar.

   //dann velocity integrieren
   //deltaTwist(w1, v1)
   //Matrix3x3 outDeltaRotation; -> is updated inside convertPlueckerToSE3components
   //FVector outDeltaTranslation; -> is updated inside convertPlueckerToSE3components 
   Matrix3x3::convertPlueckerToSE3components(
       w1_constrained, v1_constrained, outDeltaRotation, outDeltaTranslation, deltatime
   );

   //refresh, for propagation to next joint
   angularVelocity = w1;
   linearVelocity = v1;

}















MMatrix Matrix6x6::operator*(const MMatrix &worldPrev){

    //MWorld = prev * R * T <--
    MMatrix transformLocal = LocalTransform();
    MMatrix result = worldPrev * transformLocal; // lese richtung

    //save world result into cache
    SafeWorldResultCache(result);

    UpdateFloorContact(worldPrev, result);
    return result;
}

MMatrix Matrix6x6::LocalTransform(){
    MMatrix translationLocal(resultTranslation);
    MMatrix rotationLocal;
    CopyRotationTo(rotationLocal);
    MMatrix transformLocal = rotationLocal * translationLocal; //<-- lese richtung --
    return transformLocal;
}

void Matrix6x6::UpdateFloorContact(const MMatrix &prev, const MMatrix &current){
    
    contactFloor = false;
    //if (IsGrounded(prev) && IsGrounded(current))
    if (IsGrounded(prev) || IsGrounded(current)){
        contactFloor = true;
    }

    UpdateGroundConstraint(current);

}

void Matrix6x6::UpdateGroundConstraint(const MMatrix &worldResult){
    UpdateGroundConstraintPitchAndPosition();
    UpdateGroundConstraintRoll(worldResult);
}

void Matrix6x6::UpdateGroundConstraintPitchAndPosition(){
    groundContactConstraint.allowPitchRotationPositive = true;
    groundContactConstraint.allowPitchRotationNegative = true;
    groundContactConstraint.allowPositionOffsetZGrounded = true;

    if(contactFloor){
        groundContactConstraint.allowPitchRotationNegative = false;
        groundContactConstraint.allowPositionOffsetZGrounded = false;
    }
}

void Matrix6x6::UpdateGroundConstraintRoll(const MMatrix &worldResult){
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

// cache
void Matrix6x6::SafeWorldResultCache(const MMatrix &other){
    worldLocationCache = other.getTranslation();
}




// --- external transform override ---

Matrix3x3 Matrix6x6::GetRotation() const {
    return RotationSO3;
}

void Matrix6x6::CopyRotationTo(MMatrix &copyInside) const {
    std::vector<float> values = RotationSO3.Copy();
    copyInside.setRotation(values);
}


void Matrix6x6::OverrideRotation(const MMatrix &rotationMatIn){
    std::vector<float> rotationValues = rotationMatIn.CopyRotation();
    RotationSO3.Override(rotationValues);
}


void Matrix6x6::OverrideRotation(const Matrix3x3 &rotationMatIn){
    RotationSO3 = rotationMatIn;
}

void Matrix6x6::OverrideRotation(const FRotator &r){
    RotationSO3.setRotation(r);
}












/// ----- backward force -----

//makes forward pluecker and refreshes the given w and v
//transform update is saved, R and T SE3 are updated
void Matrix6x6::backwardWrench(
    FVector &moment, //n
    FVector &force,  //f
    float deltatime
){

    FVector nDelta;
    FVector fDelta;

    Matrix3x3 outDeltaRotation;
    FVector outDeltaTranslation;
    backwardDeltaWrench(
        nDelta,
        fDelta,
        deltatime
    );

    moment += nDelta;
    force += fDelta;
}


void Matrix6x6::backwardDeltaWrench(
    FVector &moment, //n
    FVector &force,  //f
    float deltatime
){

    /*
    X^T = |R^T        s(t)R^T|
          |0_3x3      R^T    |
    
    
    
        R in SO3 Gruppe
    

    j_f(n,f) = X * i_f(n,f)

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
   a.transpose();

   Matrix3x3 b1 = RotationSO3;
   b1.transpose();
   Matrix3x3 b2 = Matrix3x3::skew(translation);
   Matrix3x3 b = b2 * b1;

   Matrix3x3 c;
   c.makeZero();

   Matrix3x3 d = a;


   FVector f1 = a * moment + b * force;
   FVector n1 = c * moment + d * force;

   //constraints limitieren:
   FVector f1_constrained = f1;
   FVector n1_constrained = n1;
   applyConstraints(f1_constrained, n1_constrained); // ob hier noch unklar.

   //refresh, for propagation to next joint
   moment = f1_constrained;
   force = n1_constrained;

}




//// ----- internal torque ------

FVector Matrix6x6::Force(float mass){
    FVector totalForce(0, 0, 0);
    totalForce += GravityForce(mass);
    totalForce += NormalForce(mass);
    ShowVector(totalForce, 100.0f, FColor::Purple); //normal force is too low?
    return totalForce;

    /*
    if(!contactFloor){
        return GravityForce(mass);
    }else{
        return NormalForce(mass);
    }*/
}

FVector Matrix6x6::GravityForce(float mass){
    if(!contactFloor){
        return FVector(0, 0, -981.0f) * mass;
    }
    return FVector(0, 0, 0);
}

FVector Matrix6x6::NormalForce(float mass){
    if(contactFloor){

        FVector fg1 = GravityForce(mass) * -1.0f;

        //float antiGravity = 981.0f;
        float FnScalar = groundPenetration; //+antiGravity
        FVector FNormal = FnScalar * groundNormal.GetSafeNormal() * mass;
        FNormal += fg1;
        ShowVector(FNormal, 100.0f, FColor::Green);
        return FNormal;

    }
    return FVector(0, 0, 0);
}

FVector Matrix6x6::Torque(const FVector &force, const FVector &centerOfMass){
    //return Torque(RotationSO3, force, centerOfMass);
    return Torque(RotationSO3, force, centerOfMass);
}

FVector Matrix6x6::Torque(const Matrix3x3 &rotationSpace, const FVector &force, const FVector &centerOfMass){
    FVector comWorld = rotationSpace * centerOfMass;
    return FVector::CrossProduct(comWorld, force);
}


//// ----- GROUNDED CHECKS -----
void Matrix6x6::UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn){
    ignoreParams = ignoreParamsIn;
}

//debug
void Matrix6x6::ShowPosition(const MMatrix &other){
    ShowPosition(other.getTranslation());
}

void Matrix6x6::ShowPosition(FVector pos){
    DebugHelper::showLineBetween(
        world,
        pos,
        pos + FVector(10,0,100),
        FColor::Purple,
        1.0f
    );
}

void Matrix6x6::ShowVector(const FVector &vec, float size, FColor color){
    DebugHelper::showLineBetween(
        world,
        worldLocationCache,
        worldLocationCache + vec.GetSafeNormal() * size,
        color,
        0.1f
    );
}


//debug

bool Matrix6x6::IsGrounded(const MMatrix &prev){
    FVector t = prev.getTranslation();
    return IsGrounded(t);
}

bool Matrix6x6::IsGrounded(FVector &Start){
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


void Matrix6x6::SetGroundPenetration(bool result, const FVector &start, const FVector &hit){
    groundPenetration = 0.0f;
    if(result && BelowGround(start, hit)){ //start lower than hit, stuck in ground
        FVector groundPenetrationDirection = start - hit;
        float dot = FVector::DotProduct(groundNormal, groundPenetrationDirection.GetSafeNormal());
        if(dot <= 0.0f){
            //is inside floor
            groundPenetration = groundPenetrationDirection.Size() * dot;
        }
    }
}

void Matrix6x6::DrawGroundPenetration(const FVector &pos){
    DebugHelper::showLineBetween(world, pos, pos + FVector(0, 0, groundPenetration), FColor::Orange, 1.0f);
}

bool Matrix6x6::BelowGround(const FVector &check, const FVector &ground){
    return check.Z < ground.Z;
}
