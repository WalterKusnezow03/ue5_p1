#include "FVector6D.h"
#include "MMatrix3x3.h"

FVector6D::FVector6D(){

}

FVector6D::~FVector6D(){

}

FVector6D::FVector6D(const FVector6D &other){
    if(this != &other){
        *this = other;
    }
}
FVector6D &FVector6D::operator=(const FVector6D &other){
    if(this != &other){
        update(
            other.wx,
            other.wy,
            other.wz,
            other.vx,
            other.vy,
            other.vz
        );
    }
    return *this;
}

//upper part of vector
FVector FVector6D::getRotationVelocity(){
    FVector vec(
        wx,
        wy,
        wz
    );
    return vec;
}

//lower part of vector
FVector FVector6D::getVelocity(){
    FVector vec(
        vx,
        vy,
        vz
    );
    return vec;
}





FVector FVector6D::extractPosition(FVector &x0, float deltaTime){
    //x(t) = x0 + V * t //V ist der spatial vector (this)
    //x(t) = x0 + V.Velocity * t

    FVector xt = x0 + getVelocity() * deltaTime;
    DebugHelper::logMessage("extractedposition ", xt);

    return xt;
}

FRotator FVector6D::extractRotation(FRotator &r0, float deltaTime){

    //R(t) = R(0) * exp(wSkew * dt) //wSkew skew mat from ownRotation
    
    //exp(A, grad) = I + A + (A*A) / 2! + ...
    MMatrix3x3 rOut;

    MMatrix3x3 r0Mat(r0);

    FVector wVector = getRotationVelocity();
    MMatrix3x3 skewWMat = MMatrix3x3::skewMatrix(wVector);
    skewWMat *= deltaTime;

    int detail = 5;
    MMatrix3x3 expMat = skewWMat.expMatrix(detail);

    //M = A * B <-- lese richtung --
    rOut = r0Mat * expMat;




    return rOut.extractRotator();
}


void FVector6D::updateRotationVelocity(FVector &other){
    wx = other.X;
    wy = other.Y;
    wz = other.Z;
}
void FVector6D::updateLiniearVelocity(FVector &other){
    vx = other.X;
    vy = other.Y;
    vz = other.Z;
}



void FVector6D::update(
    float wxIn,
    float wyIn,
    float wzIn,
    float vxIn,
    float vyIn,
    float vzIn
){
    wx = wxIn;
    wy = wyIn;
    wz = wzIn;

    vx = vxIn;
    vy = vyIn;
    vz = vzIn;
}