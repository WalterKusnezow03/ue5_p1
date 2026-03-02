#include "Matrix6x6.h"

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
    resultTranslation = outDeltaTranslation + translation; //sollte so ok sein
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
    MMatrix result;

    MMatrix translationLocal(resultTranslation);
    MMatrix rotationLocal;
    std::vector<float> values = RotationSO3.Copy();
    rotationLocal.setRotation(values);


    //MWorld = prev * R * T <--
    MMatrix transformLocal = rotationLocal * translationLocal; //<-- lese richtung --
    result = worldPrev * transformLocal; //lese richtung
    return result;
}



// --- external transform override ---

Matrix3x3 Matrix6x6::GetRotation() const {
    return RotationSO3;
}

void Matrix6x6::OverrideRotation(const MMatrix &rotationMatIn){
    std::vector<float> rotationValues = rotationMatIn.CopyRotation();
    RotationSO3.Override(rotationValues);
}


void Matrix6x6::OverrideRotation(const Matrix3x3 &rotationMatIn){
    RotationSO3 = rotationMatIn;
}