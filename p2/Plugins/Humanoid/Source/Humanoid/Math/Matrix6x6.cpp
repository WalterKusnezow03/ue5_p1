#include "Matrix6x6.h"

Matrix6x6::Matrix6x6(){

}

Matrix6x6::~Matrix6x6(){

}

void Matrix6x6::setTranslation(FVector &other){
    translation = other;
    resultTranslation = other;
}


void Matrix6x6::applyConstraints(FVector &w, FVector &v){
    //kugel gelenk
    if(true){
        v = FVector(0, 0, 0);
    }
}




//makes forward pluecker and refreshes the given w and v
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
    );


    //Integrieren
    resultTranslation = outDeltaTranslation + translation; //sollte so ok sein
    RotationSO3 = RotationSO3 * outDeltaRotation; //<-- lese richtung so --

    //refresh, for propagation to next joint
    angularVelocity = w1;
    linearVelocity = v1;
}


MMatrix Matrix6x6::operator*(MMatrix &prev){
    MMatrix result;

    MMatrix translationLocal(resultTranslation);
    MMatrix rotationLocal;
    std::vector<float> values = RotationSO3.Copy();
    rotationLocal.setRotation(values);


    //MWorld = T * R * prev
    MMatrix transform = rotationLocal * translationLocal; //<-- lese richtung --
    result = prev * transform; //lese richtung
    return result;
}
