#include "NNPathFinderSocket.h"

#include "DebugPlugin/DebugHelper.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"


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

    frameNameRequest = "ANNPathFinderSFIN";
    frameNameResult = "ANNPathFinderSFRES";
    frameNameGroundThruth = "ANNPathFinderSFGT";
    
    
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

void ANNPathFinderSocket::Tick(float deltatime){
    Super::Tick(deltatime);
    actorTracker.Tick(deltatime);
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


//flags a aactor as spotted
void ANNPathFinderSocket::FlagVisible(AActor *actor){
    if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
        actorTracker.FlagVisible(actor);
    }
}


void ANNPathFinderSocket::PredictNode(
    AActor *actor,
    float radius
){
    PredictNodeDebug(actor, radius);
    return;
    if (actor)
    {
        //add if needed (filled with self location with default size buffer)
        actorTracker.AddTrackedActorIfNeeded(actor);
    }
}







/// ------ DEBUG ------
void ANNPathFinderSocket::PredictNodeDebug(
    AActor *actor,
    float radius
){
    if(actor){
        //add if needed (filled with self location with default size buffer)
        actorTracker.AddTrackedActorIfNeeded(actor);
        



        FVector location = actor->GetActorLocation();
        //FMeshedPolygon polygonData;

        FMeshedPolygonTrajectoryLayered polygonData;
        proxy.CollectPolygon(
            location,
            radius,
            polygonData
        );

        TArray<Trajectory> trajectories = actorTracker.LocalTrajectories(actor);
        polygonData.EmbedTrajectories(trajectories);

        WriteDataRequest(polygonData);


    }
}





void ANNPathFinderSocket::WriteDataRequest(
    FMeshedPolygonTrajectoryLayered &polygon
){
    TArray<uint8> buffer;
    //polygon.AppendFlagMap(buffer); //deprecated!
    polygon.AppendFlagMapAsFloat(buffer);
    polygon.AppendTimeMap(buffer);

    WriteData(frameNameRequest, buffer);
}







