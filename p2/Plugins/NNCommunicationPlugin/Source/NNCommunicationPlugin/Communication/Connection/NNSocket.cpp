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
    frameNameSend = true; //none at start
    LaunchPythonProcess("nn_server.py"); // finds working dir automatically

}

void ANNSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    CloseSharedMemory();
    Super::EndPlay(EndPlayReason);
}


void ANNSocket::OpenSharedMemory(int bytes){
    CloseSharedMemory();
    sharedMemory.Open(frameName, bytes);
    frameNameSend = false;
    // void WriteData(const TArray<uint8> &bytes);
}

void ANNSocket::CloseSharedMemory(){
    sharedMemory.CleanFrame();
}



//Tick:
//if (serverRunning && connected)
void ANNSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);

    // is connected: Tick
    if(!frameNameSend){

        //setup shared memory page inside python (if page changed)
        //message as: FrameName_bytes
        FString message = sharedMemory.SharedFrameIdentifierMessage("FRAMEID-");
        Send(message);
        frameNameSend = true;
    }
}
