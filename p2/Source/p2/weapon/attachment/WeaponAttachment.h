#pragma once
#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"

/**
 * stores the attached actor and mmatrix for the offset to the actor its attached to
 */
class P2_API WeaponAttachment{

public:
    WeaponAttachment();
    WeaponAttachment(const WeaponAttachment &other);
    WeaponAttachment &operator=(const WeaponAttachment &other);
    
    ~WeaponAttachment();

    void setup(AActor *actorin, MMatrix &other);

    AActor *attachedActorPointer();
    MMatrix &offsetMatrix();

private:
    AActor *actor = nullptr;
    MMatrix offsetMatrixForActor;
};