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
        LaunchPythonProcess(
            "NNCommunicationPlugin", 
            "nn_server.py",
            "NNCommunicationPlugin",
            "venv/bin/python"
        ); // finds working dir automatically
        //frameNames.Add("ANNSocketFrame");
    }
}

void ANNSocket::SetFlagsOnBeginPlay(){
    Super::SetFlagsOnBeginPlay();
    
}




void ANNSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    instancePtr = nullptr;
    CloseSharedMemory();
    Super::EndPlay(EndPlayReason);
}


void ANNSocket::CloseSharedMemory(){
    frameManager.ClearAllFrames();
}

void ANNSocket::WriteData(FString name, const TArray<uint8> &data){
    DebugHelper::logMessage(FString::Printf(TEXT("ANNSocket::WriteData %s %d"), *name, data.Num()));
    frameManager.WriteData(name, data);
}






//Tick:
//if (serverRunning && connected)
void ANNSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);

    frameManager.NotifyChangedFrames(this);

    /**
    ########## always send empty messages if nothing happens: otherwise TCP will be blocking in python! ##########
    ########## Fixing attempts not sucessfull yet, is a workaround! ##########
     */

    /// ----- DO NOT REMOVE -----
    //Send("EmptyData"); //MUST ALWAYS HAPPEN: OTHERWISE SOCKET IS BLOCKING IN PYTHON
    /// the python code will be stuck!!!
    /// ----- DO NOT REMOVE -----
}
