#include "NNPathFinderSocket.h"

#include "DebugPlugin/DebugHelper.h"

void ANNPathFinderSocket::MakePathFinderSocketInstance(UWorld* World)
{
    if(PathFinderNNinstance()){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ANNPathFinderSocketActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ANNPathFinderSocket* Spawned = World->SpawnActor<ANNPathFinderSocket>(
            ANNPathFinderSocket::StaticClass(), Location, Rotation, SpawnParams
        );   
        instancePtr = Spawned;
    }
}



void ANNPathFinderSocket::BeginPlay(){
    Super::BeginPlay();
    DebugHelper::logMessage("ANNPathFinderSocket BeginPlay - python");
    frameName = "ANNPathFinderSocketFrame";
    
    // /NNCommunicationPlugin/Python/nn_server.py
    //LaunchPythonProcess("NNCommunicationPlugin", "nn_server.py");

    LaunchPythonProcess("PathFinderNNExtension", "nnpathfinder_server.py"); // finds working dir automatically
}


void ANNPathFinderSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---
}
