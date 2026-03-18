#pragma once

#include "CoreMinimal.h"

class Matrix3x3;

/// @brief vector 6x1, w and v for Matrix6x6 and Joint Pluecker FK
class PLUECKERCORE_API SpatialVector {

public:
    //Vector6x1 &operator+(const Vector6x1 &other);

    //const FVector &GetWAngularVelocity();
    //const FVector &GetVLinearVelocity();

    //adds own velocity to wIn and vIn for twist propagation
    void AddVelocity(const FVector &wIn, const FVector &vIn);
    void AddOwnVelocityTo(FVector &wIn, FVector &vIn);
    

    void AddTorque(
        const FVector &torque,
        const Matrix3x3 &interiaInverse,
        float deltaTime
    );
    void AddForce(
        const FVector &force,
        float mass,
        float deltaTime
    );

    void Override(const FVector &wIn, const FVector &vIn){
        w = wIn;
        v = vIn;
    }

    void Damp(float factor);

    void OverrideLinearVelocity(FVector &vIn){
        v = vIn;
    }

    FString ToString();
    FString ToString(FString prefix);

    void AddNoise();


    void ExtractCurrentForce(
        const Matrix3x3 &interia,
        float mass,
        FVector &outN,
        FVector &outF
    );

    void SetAngularDampingFactor(float value);

private:
    FVector w = FVector(0,0,0);
    FVector v = FVector(0,0,0);
    float angularDampingFactor = 0.5f;

    float SafeDenominator(float value);
    void DebugKeepRange(FVector &vec, double limit);

    void RemoveFloatingError(FVector &vec);
    void RemoveFloatingError(double &value);

    void AddNoise(float range);
};