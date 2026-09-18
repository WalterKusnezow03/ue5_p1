#include "EntityAssetBase.h"



void AEntityAssetBase::BeginPlay(){
    Super::BeginPlay();



}

#include "p2/_world/worldLevel.h"
#include "p2/entityManager/EntityManager.h"
void AEntityAssetBase::despawn(){
    enableActiveStatus(false);
    if (EntityManager *e = AworldLevel::entityManager()){
		e->add(this);
	}
}

void AEntityAssetBase::enableActiveStatus(bool enable){
	activated = enable;
	SetActorHiddenInGame(!enable);
}