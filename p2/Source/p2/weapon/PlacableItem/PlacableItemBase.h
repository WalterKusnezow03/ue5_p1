#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weapon.h"

#include "PlacableItemBase.generated.h"

//derived from weapon in case multiple can be hold for "Reloading"
//will also be the first in hiearchy which is carried by the hands
//instead of socket
UCLASS()
class P2_API APlacableItemBase : public Aweapon {
    GENERATED_BODY()

public:
	
public:

	// Sets default values for this actor's properties (constructor)
	APlacableItemBase();

    
    // --- HAND CARRY INSTEAD OF SOCKET ---
    virtual EIKCarryType GetCarryType() override {
		return EIKCarryType::ECarryByHand;
	}

	/// @brief items attached to empty actor will receive the hand locations 
	/// from the actor by this method, if the carry type is ECarryByHand.
    virtual void UpdateLocalSceneTransformCarriedByHand(EArmType type, FVector &location, FRotator &rotation) override;

    //virtual FIKCarryInterfaceAxisConstraint &getAxisConstraint() override;

protected:
    //scene to change location and rotation from UpdateActorTransformCarriedByHand
    virtual USceneComponent *FindHandCarriedScene(EArmType type){
        return nullptr;
    }


    virtual void SetupCarriedItemAxisContraints() override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    virtual bool shootProtected(FVector Start, FVector End, teamEnum ownTeam) override;
    
    //surpressed by default on place
    virtual bool isSoundSurpressed() override {
        return true;
    }

    bool stickyProperty = false;

    virtual float PlaceRadius();
    bool IsInPlaceRadius(FVector &location);


    /// @brief MUST BE OVERRIDEN!
    /// @param location 
    /// @param normal 
    virtual void SpawnItemAtLocation(FVector &location, FVector &normal);


    //other animation for placing item
    virtual void setupKickBackAnimation();
};