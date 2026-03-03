#include "SpatialVector.h"
#include "PlueckerCore/Math/Matrix3x3.h"
#include "DebugPlugin/DebugHelper.h"


void SpatialVector::Damp(float factor){
    if(factor >= 0.0f && factor <= 1.0f){
        w *= factor;
        v *= factor;
    }
}

void SpatialVector::AddVelocity(const FVector &wIn, const FVector &vIn){
    w += wIn;
    v += vIn;
    Damp(0.5f);
}
void SpatialVector::copy(FVector &wIn, FVector &vIn) const{
    wIn = w;
    vIn = v;
}

void SpatialVector::AddTorque(
    const FVector &torque, 
    const Matrix3x3 &interiaInverse,
    float deltaTime
){
    if(torque.Size() < 0.0001f){
        return;
    }
    //tau = I * a_theta
    //a_theta = I^-1 * tau

    /*
    $$
    a_{angular} = I^{-1} \cdot \tau
    $$
    $$
    \omega = \omega_{old} + a_{angular} \cdot \Delta t
    $$
    */
    //Matrix3x3 interiaInverse = interia.jordanInverse();
    FVector angularAcceleration = interiaInverse * torque;
    w += angularAcceleration * deltaTime;
    //DebugHelper::showScreenMessage("Spatial Torque ", torque, FColor::Red);
    if(false){
        DebugHelper::showScreenMessage("Spatial Angular ", w);
    }

    //debug
    if(false){DebugKeepRange(w, 2.0f);}

    Damp(0.5f);
}

void SpatialVector::DebugKeepRange(FVector &vec, double limit){
    vec.X = std::min(vec.X, limit);
    vec.Y = std::min(vec.Y, limit);
    vec.Z = std::min(vec.Z, limit);

    vec.X = std::max(vec.X, -limit);
    vec.Y = std::max(vec.Y, -limit);
    vec.Z = std::max(vec.Z, -limit);
}


void SpatialVector::AddForce(
    const FVector &force, //f = m * a
    float mass, 
    float deltaTime
){
    //F = m * a
    //a = F / m
    mass = std::abs(SafeDenominator(mass));

    FVector a = force / mass;
    v += a * deltaTime;
}

float SpatialVector::SafeDenominator(float value){
    float sign = value < 0.0f ? -1.0f : 1.0f;
    float valueAbs = std::abs(value);
    valueAbs = std::max(valueAbs, 1.0f);
    return sign * valueAbs;
}



// --- not known if needed ! ---

void SpatialVector::RemoveFloatingError(FVector &vec){
    RemoveFloatingError(vec.X);
    RemoveFloatingError(vec.Y);
    RemoveFloatingError(vec.Z);
}

void SpatialVector::RemoveFloatingError(double &value){
    float abs = std::abs(value);
    if(abs <= 0.01f){
        value = 0.0f;
    }
}
