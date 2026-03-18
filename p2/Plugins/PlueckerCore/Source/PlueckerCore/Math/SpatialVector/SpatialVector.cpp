#include "SpatialVector.h"
#include "PlueckerCore/Math/Matrix3x3.h"
#include "DebugPlugin/DebugHelper.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraintBase.h"


/// @brief daming by 0.5 added, to ensure the joint doesnt spin forever
/// adds certainly a good visual quality, is WANTED like this!
/// @param factor 
void SpatialVector::Damp(float factor){
    if(factor >= 0.0f && factor <= 1.0f){
        w *= factor; //ang vel only.
        //v *= factor;
    }
}

void SpatialVector::SetAngularDampingFactor(float value){
    float maxDamp = 0.5f; //TESTED VALUE, IS THE MOST STABLE, DO NOT CHANGE, is very good. Looks good.
    value = std::max(maxDamp, value);
    value = std::min(0.0f, value);
    angularDampingFactor = value;
}

void SpatialVector::AddVelocity(const FVector &wIn, const FVector &vIn){
    w += wIn;
    v += vIn;

    /// --- DO NOT REMOVE --- -> 0.5 value looks good, is tested!
    /// @brief daming by 0.5 added, to ensure the joint doesnt spin forever
    /// adds certainly a good visual quality, is WANTED like this!
    //float factor = 0.5f;
    Damp(angularDampingFactor);
}


void SpatialVector::AddOwnVelocityTo(FVector &wIn, FVector &vIn){
    wIn += w;
    vIn += v;
}


void SpatialVector::AddTorque(
    const FVector &torque, 
    const Matrix3x3 &interiaInverse,
    float deltaTime
){
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
    DebugKeepRange(w, 1000.0f);

    Damp(angularDampingFactor);
    //Damp(0.5f);
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

    //very effective. Keep.
    v *= 0.9f;
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



FString SpatialVector::ToString(){
    return FString::Printf(
        TEXT("SpatialVelocity w(%.2f %.2f %.2f) v(%.2f %.2f %.2f)"),
        w.X, w.Y, w.Z,
        v.X, v.Y, v.Z
    );
}
FString SpatialVector::ToString(FString prefix){
    return prefix + ToString();
}



void SpatialVector::AddNoise(){
    AddNoise(0.1f); //0.01
}
void SpatialVector::AddNoise(float range){
     w += FVector(
        FMath::FRandRange(-range,range),
        FMath::FRandRange(-range,range),
        FMath::FRandRange(-range,range)
    );
}




void SpatialVector::ExtractCurrentForce(
    const Matrix3x3 &interia,
    float mass,
    FVector &outN,
    FVector &outF
){
    //F = Ia * v x Iv //I 6x6, v 6x1, a negotiable, inetgrated before.
    //I [Icm , 0]
    //  [0,   m1]

    //...
    
    // vereinfacht ohne I a

    /*$$
    F_{motion} =
    \begin{bmatrix}
        [w]_x & 0 \\
        [v]_x & [w]_x
    \end{bmatrix}
    \cdot 
    \begin{bmatrix}
        I \cdot  w \\ 
        m1 \cdot  v
    \end{bmatrix}
    $$*/
    Matrix3x3 adT_a = Matrix3x3::skew(w);
    Matrix3x3 adT_b;
    adT_b.makeZero();

    Matrix3x3 adT_c = Matrix3x3::skew(v);
    Matrix3x3 adT_d = adT_a;

    FVector Iw = interia * w;
    FVector mv = mass * v;

    FVector n = adT_a * Iw + adT_b * mv;
    FVector f = adT_c * Iw + adT_d * mv;

    FJointConstraintBase::ApplyConstraintNaN(n);
    FJointConstraintBase::ApplyConstraintNaN(f);

    outN = n;
    outF = f;
}
