#include "PlacableItemBase.h"



APlacableItemBase::APlacableItemBase() : Super(){

};

void APlacableItemBase::BeginPlay(){
    Super::BeginPlay();

}

void APlacableItemBase::Tick(float deltatime){
    Super::Tick(deltatime);

}



bool APlacableItemBase::shootProtected(FVector Start, FVector End, teamEnum ownTeam){
    if(Super::shootProtected(Start, End, ownTeam)){

        //place item at latest hit location
        FVector location = LatestHitLocation();
	    FVector normal = LatestHitNormal();

        DebugHelper::showLineBetween(GetWorld(), location, GetActorLocation(), FColor::Red);

        if(IsInPlaceRadius(location)){
            SpawnItemAtLocation(location, normal);
            return true;
        }
    }
    return false;
}



bool APlacableItemBase::IsInPlaceRadius(FVector &location){
    float dist = FVector::DistSquared(location, GetActorLocation());
    float maxDist = PlaceRadius() * PlaceRadius();
    return dist <= maxDist;
}

float APlacableItemBase::PlaceRadius(){
    return 300.0f;
}

//pure virtual like.
void APlacableItemBase::SpawnItemAtLocation(FVector &location, FVector &normal){
    //pure virtual for now.
}



void APlacableItemBase::setupKickBackAnimation(){
    ClearKickbackAnimation();
    
	int kickFrontDistance = 10; // 3cm
	actorKickBackAnim.addFrame(
		FVector(0, 0, 0),
		0.0f, // time to prev frame
		false);
	actorKickBackAnim.addFrame(
		FVector(kickFrontDistance, 0, 0), //x forward
		cooldownTime() * 0.1f, //time to prev frame
		false
	);
	actorKickBackAnim.addFrame(
		FVector(0, 0, 0),
		cooldownTime() * 0.9f, //time to prev frame
		false
	);

}






/// @brief items attached to empty actor will receive the hand locations 
/// from the actor by this method, if the carry type is ECarryByHand.
void APlacableItemBase::UpdateLocalSceneTransformCarriedByHand(
    EArmType type, 
    FVector &locationLocal, 
    FRotator &rotationLocal
){
    //exapmple implementation
    if(GetCarryType() == EIKCarryType::ECarryByHand){
        if(USceneComponent *item = FindHandCarriedScene(type)){
            item->SetRelativeLocation(locationLocal);
            item->SetRelativeRotation(rotationLocal);
        }
    }
}




void APlacableItemBase::SetupCarriedItemAxisContraints(){
    carryInterfaceAxisConstraint.SetupNone();
    carryInterfaceAxisConstraint.LockPositionConstraint();
}