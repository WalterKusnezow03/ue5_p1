#include "NNPathFinderSocket.h"

#include "DebugPlugin/DebugHelper.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"


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
    actorTracker.Setup(0.5f, 20);

    DebugHelper::logMessage("ANNPathFinderSocket BeginPlay - python");

    frameNameRequest = "ANNPathFinderSFIN";
    frameNameResult = "ANNPathFinderSFRES";
    frameNameGroundThruth = "ANNPathFinderSFGT";
    frameNameBatch = "ANNPathFinderSFB";

    //Plugins/NNCommunicationPlugin/Source/NNCommunicationPlugin/Python/venv/bin/python
    LaunchPythonProcess(
        "PathFinderNNExtension", 
        "nnpathfinder_server.py",
        "NNCommunicationPlugin",
        "venv/bin/python"
    ); // finds working dir automatically

    task.Reset();
    batchTask.Load();
}

void ANNPathFinderSocket::Tick(float deltatime){
    Super::Tick(deltatime);
    LoadBatchIfNotDoneYet();

    actorTracker.Tick(deltatime);
    
    //tick in connected? samples can be collected without.
    TickTask();
    
    
    TickReadDataResult();
    //DebugHelper::showScreenMessage("ANNPathFinderSocket::Tick", FColor::Cyan);


    //if a task will be completed: reload next task automatically.
    PredictNextTask();
}

void ANNPathFinderSocket::LoadBatchIfNotDoneYet(){
    if(trainingStartAllowed && !batchTask.BatchPrepared()){
        DebugHelper::logMessage("ANNPathFinderSocket::LoadBatchIfNotDoneYet");
        TArray<uint8> buffer;
        batchTask.PrepareBinary(buffer);
        if (buffer.Num() > 0)
        {
            WriteData(frameNameBatch, buffer);
        }
    }
}

void ANNPathFinderSocket::SetTrainingAllowed(){
    trainingStartAllowed = true;
}





void ANNPathFinderSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---
    //TickTask();
    DebugHelper::showScreenMessage("ANNPathFinderSocket::TickSocketConnected", FColor::Cyan);
}

void ANNPathFinderSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    task.EndSave();
    SaveHeatMapsOnEndPlay();
    batchTask.EndSave();
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


// ---- subrscribe observer pattern ----
void ANNPathFinderSocket::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
    }
    if(interfaceNotify && actor){
        requests.Subscribe(interfaceNotify, actor);
    }
}





void ANNPathFinderSocket::PredictNode(
    //AActor *actor
    FPathFinderNNRequestPackage *package
){
    


    //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION - TRY A");
    //one task at a time for now.
    if(task.IsValid()){
        if(!task.TaskCompleted()){
            return;
        }
    }




    if(!package){
        return;
    }
    AActor *actor = package->GetActor();

    DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION - TRY B");

    if (actor){
        //add if needed (filled with self location with default size buffer)
        actorTracker.AddTrackedActorIfNeeded(actor);
        task.Setup(actorTracker.FindIfTracked(actor));
        if(task.IsValid()){
            //on task start: embed enemy postions for this actor.
            task.EmbedEnemyPositionsAndVision(*package);

            //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION");
            //DebugHelper::showScreenMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION", FColor::Red);
            TArray<uint8> requestBinary;
            task.PrepareRequestBinary(requestBinary);

            //if data invalid, task is resettet
            if(task.IsValid()){
                WriteDataRequest(requestBinary, task.ResultGridSizeBytes());
            }
        }
    }
}


//call this once a task is completed
void ANNPathFinderSocket::PredictNextTask(){
    if(requests.HasTasks()){
        
        /*//get next prediction actor
        if(AActor *front = requests.frontActor()){
            PredictNode(front); //if a task is still running right now, it will be discared.
            //the list stays the same.
        }*/
        if(FPathFinderNNRequestPackage *package = requests.frontPackage()){
            PredictNode(package);
        }
    }
}

void ANNPathFinderSocket::TickTask(){
    if(task.IsValid() == false){
        //DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask NOT VALID", FColor::Red);
        return;
    }
    if(!task.WaitingForGroundTruth()){
        return;
    }

    //DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask", FColor::Red);

    TArray<uint8> groundTruthBinary;
    task.TickVisiblityCheckAndPrepareGroundTruthBinary(groundTruthBinary);
    if(groundTruthBinary.Num() > 0){
        DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask OK", FColor::Orange);

        //since data and ground truth are ready:
        //add to batch
        batchTask.AddSample(task.GetPolygonData());

        WriteDataGroundTruth(groundTruthBinary);
    }
}



void ANNPathFinderSocket::WriteDataRequest(TArray<uint8> &buffer, int resultBytes){
    FString message = FString::Printf(
        TEXT("ANNPathFinderSocket::WriteDataRequest %d, Result Bytes Expected %d"),
        buffer.Num(),
        resultBytes
    );
    DebugHelper::logMessage(message);
    //DebugHelper::showScreenMessage(message, FColor::Red);
    if(buffer.Num() > 0 && resultBytes > 0){
        
        // ---- result must exists to ask the NN for a result! (is not created in python!) ----
        frameManager.MakeSureExists(frameNameResult, resultBytes);
        WriteData(frameNameRequest, buffer);
    }else{
        task.Reset();
    }
}

void ANNPathFinderSocket::WriteDataGroundTruth(
    TArray<uint8> &buffer
){
    if(buffer.Num() > 0){
        FString message = FString::Printf(TEXT("ANNPathFinderSocket::WriteDataGroundTruth %d"),buffer.Num());
        //DebugHelper::showScreenMessage(message, FColor::Red);
        DebugHelper::logMessage(message);
        WriteData(frameNameGroundThruth, buffer);
    }
    
}


void ANNPathFinderSocket::OnReceivePythonPrint(FString Output){
    Super::OnReceivePythonPrint(Output);
   
    if(false){
        //DebugHelper::showScreenMessage("ANNPathFinderSocket::ReceivePythonPrint ", Output, FColor::Red);
        DebugHelper::logMessage("ANNPathFinderSocket::PYTHON ", Output);
        return;
    }
}



void ANNPathFinderSocket::ReadDataResult(){
    
    /// create heat map from prediction
    
    //this a float buffer just like NN input
    TArray<uint8> buffer;
    frameManager.TryReadDataTo(frameNameResult, buffer);
    //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST FINISH Arrived! ResulDataSize", buffer.Num());

    task.GenerateMapFromPredicitontBytes(buffer);
    
    //create result for queue ------> TODO!
    TArray<FVector> positions;
    task.GenerateAndNotifyResultPositions(requests.frontPackage());
    requests.PopFront();



    Image image;
    FMeshedPolygonColorAttributes attributes(
        FColor(0, 0, 255, 255),     // FColor colorMinHeatIn,
        FColor(255, 0, 0, 255),     // FColor colorMaxHeatIn,
        FColor(255, 255, 255, 255), // FColor colorPolygonFlaggedIn,
        FColor(FColor::Cyan),       // FColor colorViewGridIn,
        FColor(FColor::Yellow),     // FColor colorTrjacetoryIn,
        FColor(0, 255, 0, 255)      // FColor playerPosResultIn
    );

    task.ColoredHeatMap(
        image, //Image &image,
        attributes
    );
    
    heatMaps.Add(image);
}

void ANNPathFinderSocket::TickReadDataResult(){
    if(task.TaskCompleted() == false){
        //DebugHelper::showScreenMessage("ANNPathFinderSocket::Tick Wait for Result", FColor::Cyan);
        //--> prevent python message:
        //if marked ready:
            //copy result:
            //flag not ready
        if(frameManager.TryReadReadyFlag(frameNameResult)){
            ReadDataResult();
            frameManager.MarkReadyFalse(frameNameResult); //mark as read

            FString message = "ANNPathFinderSocket::REQUEST FINISH Arrived IMPLICIT!";
            DebugHelper::logMessage(message);

            //DebugHelper::showScreenMessage(message, FColor::Red);
            task.Reset();
        }
    }
}






#include "StoragePlugin/Storage/ImageData/ImageWriter/ImageWriter.h"
void ANNPathFinderSocket::SaveHeatMapsOnEndPlay(){
    
    DebugHelper::logMessage("ANNPathFinderSocket::Save HeatMaps ", heatMaps.Num());
    ImageWriter::SaveImagesAsPngFromName(
        heatMaps,
        "NNPathFinderHeatMaps",
        "heatMap"
    );
    heatMaps.Empty();
}
