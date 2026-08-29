#include "ActorAssetByString.h"

AActor *FActorAssetByString::SpawnActor(UWorld *world) const{
    FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f);
    return SpawnActor(world, SpawnLocation);
}

AActor *FActorAssetByString::SpawnActor(UWorld *world, FVector SpawnLocation) const{
    if (SpawnClass && world)
    {
        FRotator SpawnRotation = FRotator::ZeroRotator;
        AActor* NewActor = world->SpawnActor<AActor>(SpawnClass, SpawnLocation, SpawnRotation);
        return NewActor;
    }
    return nullptr;
}



bool FActorAssetByString::NameEqualsIgnoreCase(FString &other) const{
    if(nameOfAsset.Len() > 0){
        return nameOfAsset.Equals(other, ESearchCase::IgnoreCase);
    }
    return false;
}



//the spawned actors will be appended to array
void FActorAssetByString::SpawnActors(UWorld *world, const TArray<FVector> &locations, TArray<AActor*> &outActors) const {
    if(world){
        for (int i = 0; i < locations.Num(); i++){
            AActor *spawned = SpawnActor(world, locations[i]);
            if(spawned){
                outActors.Add(spawned);
            }
        }
    }
}
