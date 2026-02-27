#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/PlacableItem/PlacableItemBase.h"
#include "p2/weapon/ExplosiveHelper/ExplosiveHelper.h"

#include "GameCore/interfaces/DamageInterface/Damageinterface.h"

#include "ExplosiveDamagableBase.generated.h"

UCLASS()
class P2_API AExplosiveDamagableBase : public APlacableItemBase, public IDamageinterface{
    GENERATED_BODY()

public:
    AExplosiveDamagableBase();

    

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    //override if needed
    virtual void Detonate();


    //change damage / radius vars on derived beginplay to override
    void SetupExplosiveHelperOnBeginPlay();

    //release to object pool
    void release();

    ExplosiveHelper explosiveHelper;

    int EXPLOSION_RADIUS = 10000; //20 000 --> (20 * 100 = 20m)
	int DAMAGE = 40;
	int DAMAGE_RADIUS = 5000;


    //TODO: RESET BY ENTITY MANAGER
    bool isDetonated = false;
    void ResetDenonatedStatus();


public:
    //damage interface derived
	virtual void takedamage(FCustomHitResult &result) override;

    virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

    teamEnum team = teamEnum::neutralTeam;


};