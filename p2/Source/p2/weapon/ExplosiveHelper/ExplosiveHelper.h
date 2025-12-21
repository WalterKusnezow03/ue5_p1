#pragma once

#include "CoreMinimal.h"

/// @brief shared functionality for grenades, c4, rockets
class P2_API ExplosiveHelper {

public:
    ExplosiveHelper();
    ~ExplosiveHelper();

    void Setup();
    
    void Setup(
        int explosionRadiusIn,
        int damageIn,
        int damageRadiusIn
    );

    void detonate(
        FVector &location,
        UWorld *world,
        int explosionRadiusIn,
        int damageIn,
        int damageRadiusIn
    );

    void detonate(FVector &location, UWorld *world);

private:


    int EXPLOSION_RADIUS = 10000; //20 000 --> (20 * 100 = 20m)
	int DAMAGE = 40;
	int DAMAGE_RADIUS = 5000;



};