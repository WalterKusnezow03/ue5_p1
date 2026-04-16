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
    
    //launch only if not blocked
    if(false){
        LaunchPythonProcess("NNCommunicationPlugin", "nn_server.py"); // finds working dir automatically
        frameName = "ANNSocketFrame";
    }
}

void ANNSocket::SetFlagsOnBeginPlay(){
    Super::SetFlagsOnBeginPlay();
    frameNameSend = true;
}




void ANNSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    instancePtr = nullptr;
    CloseSharedMemory();
    Super::EndPlay(EndPlayReason);
}


void ANNSocket::OpenSharedMemory(int bytes){
    if(frameName.Len() <= 0){
        return;
    }

    CloseSharedMemory();
    sharedMemory.Open(frameName, bytes);
    frameNameSend = false;
    // void WriteData(const TArray<uint8> &bytes);
}

void ANNSocket::CloseSharedMemory(){
    sharedMemory.CleanFrame();
}


void ANNSocket::WriteData(const TArray<uint8> &data){
    //size changed: new page!

    if(sharedMemory.SizeChanged(data.Num())){
        CloseSharedMemory();
        OpenSharedMemory(data.Num());
    }
    sharedMemory.WriteData(data);
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
