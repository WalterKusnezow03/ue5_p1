#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weapon.h"

#include "PlacableItemBase.generated.h"

//derived from weapon in case multiple can be hold for "Reloading"
UCLASS()
class P2_API APlacableItemBase : public Aweapon {
    GENERATED_BODY()

public:
	
public:

	// Sets default values for this actor's properties (constructor)
	APlacableItemBase();



protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    virtual bool shootProtected(FVector Start, FVector End, teamEnum ownTeam) override;
    
    //surpressed by default
    virtual bool isSoundSurpressed() override {
        return true;
    }

    bool stickyProperty = false;

    virtual float PlaceRadius();
    bool IsInPlaceRadius(FVector &location);

    virtual void SpawnItemAtLocation(FVector &location, FVector &normal);
};