#pragma once

#include "p2/weapon/weapon.h"

#include "throwableWeaponBase.generated.h"

//new class for hand throwable items!

/// @brief Base class for a throwable item
UCLASS()
class P2_API AthrowableWeaponBase : public Aweapon {
    GENERATED_BODY()

public:
    // --- HAND CARRY INSTEAD OF SOCKET ---
    virtual EIKCarryType GetCarryType() override {
		return EIKCarryType::ECarryByHand;
	}



protected:
    UPROPERTY()
    UPrimitiveComponent *rootMesh = nullptr;

    UPROPERTY(EditAnywhere, Category="throwableProperty")
    float throwableVelocity = 1000.0f;

    //must be assigned to find the thrown mesh on beginPlay
    UPROPERTY(EditAnywhere, Category="throwableProperty")
    FString thrownMeshName;

    void SetPhysicsEnabled(bool flag);

    FVector PhysicsMeshLocation();
    FVector PhysicsMeshVelocityDirection();
    
public:
   

    //static AthrowableWeaponBase *MakeInstance()


    // Only throw on long hold.

    virtual void MarkForApplyImpulse(FVector &direction) override;

    //shoot override to flip logic
    virtual void shoot() override;
    virtual void shootBot(FVector target) override;
    virtual void releaseShoot() override;

    //scene components must be updated by carried item by hand.
    void UpdateLocalSceneTransformCarriedByHand(
        EArmType type,
        FVector &locationLocal,
        FRotator &rotationLocal
    );

protected:
    virtual USceneComponent *FindHandCarriedScene(EArmType type);
    void ResetPhysics();

    virtual void BeginPlay() override;
    void FindThrowableMeshOnBeginPlay();

    virtual void Tick(float Deltatime) override;
    void ApplyMarkedImpulse();

    bool wasMarkedForThrow = false;
    FVector throwDirection;

    void DrawLocation(float deltatime);



   
};
