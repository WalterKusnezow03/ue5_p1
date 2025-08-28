#include "worldLevelBase.h"

/// @brief world pointer to use for easier UWorld pointer from non aactor derived classes.
UWorld *worldLevelBase::worldPointer = nullptr;

TArray<FVector> worldLevelBase::outpostsToCreate;

void worldLevelBase::addOutpostAt(FVector &pos){
    outpostsToCreate.Add(pos);
}


// ---- TODO! ---- ALSO INIT MUSt Be DERIVED!
void worldLevelBase::EndPlay(){
    ResetWorldPointer(); 
}

void worldLevelBase::SetWorld(UWorld *world){
    worldPointer = world;
}

void worldLevelBase::ResetWorldPointer(){
    worldPointer = nullptr;
}

UWorld *worldLevelBase::GetWorld(){
    return worldPointer;
}