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


    //Plugins/NNCommunicationPlugin/Source/NNCommunicationPlugin/Python/venv/bin/python
    LaunchPythonProcess(
        "PathFinderNNExtension", 
        "nnpathfinder_server.py",
        "NNCommunicationPlugin",
        "venv/bin/python"
    ); // finds working dir automatically
}


void ANNPathFinderSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---
}


void ANNPathFinderSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    proxy.EndSave(); //save images (debug)
    Super::EndPlay(EndPlayReason);
}





//// ---- interaction ----

void ANNPathFinderSocket::PredictNode(
    FVector playerPos,
    float radius
){
    FMeshedPolygon polygonData;
    proxy.CollectPolygon(
        playerPos,
        radius,
        polygonData
    );
    WriteData(polygonData);
}

void ANNPathFinderSocket::WriteData(FMeshedPolygon &polygon){
    TArray<uint8> buffer;
    polygon.AppendFlagMap(buffer);

    WriteData(buffer);
}







