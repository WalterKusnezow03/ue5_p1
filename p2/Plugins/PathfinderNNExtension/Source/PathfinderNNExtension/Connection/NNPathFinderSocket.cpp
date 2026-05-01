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
    actorTracker.Setup(0.5f, 20);

    debugMessageEnabled = false;
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
    //TickTask();
    TickReadDataResult();
    //DebugHelper::showScreenMessage("ANNPathFinderSocket::Tick", FColor::Cyan);
}

void ANNPathFinderSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---
    TickTask();
    DebugHelper::showScreenMessage("ANNPathFinderSocket::TickSocketConnected", FColor::Cyan);
}

void ANNPathFinderSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    proxy.EndSave(); //save images (debug)
    SaveHeatMapsOnEndPlay();
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
    AActor *actor
){
    //one task at a time for now.
    if(!task.TaskCompleted()){
        return;
    }
    //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION");

    if (actor){
        //add if needed (filled with self location with default size buffer)
        actorTracker.AddTrackedActorIfNeeded(actor);
        task.Setup(actorTracker.FindIfTracked(actor));
        if(task.IsValid()){
            //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION");
            DebugHelper::showScreenMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION", FColor::Red);
            TArray<uint8> requestBinary;
            task.PrepareRequestBinary(requestBinary);

            //if data invalid, task is resettet
            if(task.IsValid()){
                WriteDataRequest(requestBinary, task.ResultGridSizeBytes());
            }
        }
        
        // FMeshedPolygonTrajectoryLayered &data = GetPolygonData();
    }

    //PredictNodeDebug(actor, radius);
    //return;

}

void ANNPathFinderSocket::TickTask(){
    if(task.IsValid() == false){
        return;
    }
    if(!task.WaitingForGroundTruth()){
        return;
    }

    TArray<uint8> groundTruthBinary;
    task.TickVisiblityCheckAndPrepareGroundTruthBinary(groundTruthBinary);
    if(groundTruthBinary.Num() > 0){
        WriteDataGroundTruth(groundTruthBinary);
    }

    //if(task.WaitingForGroundTruth()){
    /*if(task.TaskCompleted() == false){
        TArray<uint8> groundTruthBinary;
        task.TickVisiblityCheckAndPrepareGroundTruthBinary(groundTruthBinary);
        if(groundTruthBinary.Num() > 0){
            WriteDataGroundTruth(groundTruthBinary);
        }
    }*/
}



void ANNPathFinderSocket::WriteDataRequest(TArray<uint8> &buffer, int resultBytes){
    FString message = FString::Printf(
        TEXT("ANNPathFinderSocket::WriteDataRequest %d, Result Bytes Expected %d"),
        buffer.Num(),
        resultBytes
    );
    //DebugHelper::logMessage(message);
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
        //FString message = FString::Printf(TEXT("ANNPathFinderSocket::WriteDataGroundTruth %d"),buffer.Num());
        //DebugHelper::showScreenMessage(message, FColor::Red);
        //DebugHelper::logMessage(message);
        WriteData(frameNameGroundThruth, buffer);
    }
    
}


void ANNPathFinderSocket::OnReceivePythonPrint(FString Output){
    //Super::OnReceivePythonPrint(Output);
    DebugHelper::showScreenMessage("ANNPathFinderSocket::ReceivePythonPrint ", Output, FColor::Red);
    if(false){
        DebugHelper::logMessage("ANNPathFinderSocket::PYTHON ", Output);
        return;
    }

    /*if(Output.Contains("RequestFinish")){
        //waiting for ground truth must be flagged true
        FString message = "ANNPathFinderSocket::REQUEST FINISH Arrived! ReceivePythonPrint";
        DebugHelper::logMessage(message);
        DebugHelper::showScreenMessage(message, FColor::Red);

        ReadDataResult();   
        task.Reset();
    }*/
}



void ANNPathFinderSocket::ReadDataResult(){
    
    
    //this a float buffer just like NN input
    TArray<uint8> buffer;
    frameManager.TryReadDataTo(frameNameResult, buffer);
    //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST FINISH Arrived! ResulDataSize", buffer.Num());

    task.GenerateMapFromResultBytes(buffer);
    Image image;
    task.ColoredHeatMap(
        image, //Image &image,
        FColor(0, 0, 255, 255), //FColor colorMin,
        FColor(255, 0, 0, 255), //FColor colorMax,
        FColor(255,255,255,255), //FColor colorPolygonFlagged,
        FColor(255,0,255,255), //FColor colorTrjacetory,
        FColor(0,255,0,255) //FColor playerPosResult
    );
    heatMaps.Add(image);
}

void ANNPathFinderSocket::TickReadDataResult(){
    if(task.TaskCompleted() == false){
        DebugHelper::showScreenMessage("ANNPathFinderSocket::Tick Wait for Result", FColor::Cyan);
        //--> prevent python message:
        //if marked ready:
            //copy result:
            //flag not ready
        if(frameManager.TryReadReadyFlag(frameNameResult)){
            ReadDataResult();
            frameManager.MarkReadyFalse(frameNameResult); //mark as read

            FString message = "ANNPathFinderSocket::REQUEST FINISH Arrived IMPLICIT!";
            DebugHelper::logMessage(message);
            DebugHelper::showScreenMessage(message, FColor::Red);
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
