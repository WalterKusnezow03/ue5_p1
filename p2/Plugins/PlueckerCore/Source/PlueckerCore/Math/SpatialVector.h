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
    void copy(FVector &wIn, FVector &vIn) const;

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

    void Damp(float factor);

private:
    FVector w;
    FVector v;

    float SafeDenominator(float value);
    void DebugKeepRange(FVector &vec, double limit);

    void RemoveFloatingError(FVector &vec);
    void RemoveFloatingError(double &value);
};