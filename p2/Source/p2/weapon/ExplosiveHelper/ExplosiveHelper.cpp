#include "ExplosiveHelper.h"
#include "p2/entityManager/AlertManager.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"

ExplosiveHelper::ExplosiveHelper(){
    Setup();
}

ExplosiveHelper::~ExplosiveHelper(){

}

void ExplosiveHelper::Setup(){
    Setup(10000, 40, 5000);
}

void ExplosiveHelper::Setup(
    int explosionRadiusIn,
    int damageIn,
    int damageRadiusIn
){
    EXPLOSION_RADIUS = std::abs(explosionRadiusIn); 
	DAMAGE = std::abs(damageIn);
    DAMAGE_RADIUS = std::abs(damageRadiusIn);
}

void ExplosiveHelper::detonate(
    FVector &location, 
    UWorld *world,
    int explosionRadiusIn,
    int damageIn,
    int damageRadiusIn
){
    Setup(explosionRadiusIn, damageIn, damageRadiusIn);
    detonate(location, world);
}

/// @brief detonates the grenade
void ExplosiveHelper::detonate(FVector &location, UWorld *world){
    if(world){
        AlertManager::damageAndAlertInArea(world, location, EXPLOSION_RADIUS, DAMAGE, DAMAGE_RADIUS);
        DebugHelper::showScreenMessage("explosive detonate"); //works as expected

        //visual explosion
        if(EntityManager *e = AworldLevel::entityManager()){
            e->createExplosion(world, location);
        }
    }
}