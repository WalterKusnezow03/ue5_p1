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
    return 200.0f;
}


void APlacableItemBase::SpawnItemAtLocation(FVector &location, FVector &normal){
    //pure virtual for now.
}