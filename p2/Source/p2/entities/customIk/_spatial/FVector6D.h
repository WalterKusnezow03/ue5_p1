#pragma once

#include "CoreMinimal.h"

class P2_API FVector6D {

public:
    FVector6D();
    ~FVector6D();

    FVector6D(const FVector6D &other);
    FVector6D &operator=(const FVector6D &other);

    FVector extractPosition(FVector &x0, float deltaTime);
    FRotator extractRotation(FRotator &r0, float deltaTime);

    FVector getRotationVelocity();
    FVector getVelocity();

    void update(
        float wxIn,
        float wyIn,
        float wzIn,
        float vxIn,
        float vyIn,
        float vzIn
    );

    void updateRotationVelocity(FVector &other);
    void updateLiniearVelocity(FVector &other);

private:
    // winkel geschwindigkeiten
    float wx = 0.0f;
    float wy = 0.0f;
    float wz = 0.0f;

    //linieare geschwindigkeiten
    float vx = 0.0f;
    float vy = 0.0f;
    float vz = 0.0f;



};