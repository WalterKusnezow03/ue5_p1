#include "CommunicationInterface.h"
#include "CoreMinimal.h"

FVector CommunicationInterface::playerPosition = FVector::ZeroVector;


void CommunicationInterface::UpdatePlayerPosition(FVector &position){
    CommunicationInterface::playerPosition = position;
}