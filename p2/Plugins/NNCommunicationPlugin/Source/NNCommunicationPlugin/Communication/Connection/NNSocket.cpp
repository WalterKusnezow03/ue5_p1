#include "NNSocket.h"

ANNSocket *ANNSocket::instancePtr = nullptr;

ANNSocket::ANNSocket() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

void ANNSocket::MakeInstance(UWorld* World)
{
    if(bDEBUGBLOCK_LAUNCH){
        return;
    }
    if(instancePtr){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ANNSocketActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ANNSocket* Spawned = World->SpawnActor<ANNSocket>(ANNSocket::StaticClass(), Location, Rotation, SpawnParams);   
        instancePtr = Spawned;
    }
}




void ANNSocket::BeginPlay(){
    Super::BeginPlay();
    LaunchPythonProcess("nn_server.py");
    
}



void ANNSocket::Tick(float deltatime){
    Super::Tick(deltatime);
    

    if(IsConnected()){
        //debug some data
        TArray<float> data = {8, 4, 3, 1};
        typeDataRandomNum++;
        typeDataRandomNum %= 3;

        Send(typeDataRandomNum, data);
    }
    LogPythonMessages();
}

