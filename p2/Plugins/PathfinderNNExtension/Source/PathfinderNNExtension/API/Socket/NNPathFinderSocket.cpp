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


#include "PathfinderNNExtension/Settings/NNSetting.h"

void ANNCommunicationSocket::OnBeginPlaySetupHandleStructure(){
    requestHandle = new NNRequestHandle();
    requestTickData = new FNNRequestHandleTickDataBase();

    
}

void ANNCommunicationSocket::OnBeginPlaySetupFrameNames(){
    //just as used with python process!
    //ANNPathFinderSF means ANNPathFinderSharedFrame
    frameNameRequest = "ANNPathFinderSFIN";
    frameNameResult = "ANNPathFinderSFRES";
    frameNameGroundThruth = "ANNPathFinderSFGT";
    frameNameBatch = "ANNPathFinderSFB";
    frameNameNNType = "ANNPathFinderSFNN";
}

void ANNCommunicationSocket::OnBeginPlayLaunchPython(){
    LaunchPythonProcess(
        "PathFinderNNExtension", 
        "nnpathfinder_server.py", //local nnpathfinder_server.py, from this plugin
        "NNCommunicationPlugin", //hier ist python installiert.
        "venv/bin/python"
    ); 
}

NNRequestHandle *ANNPathFinderSocket::GetHandle(){
    if(requestHandle){
        return static_cast<NNRequestHandle *>(requestHandle);
    }
    return nullptr;
}

void ANNPathFinderSocket::BeginPlay(){
    Super::BeginPlay();

    if(NNRequestHandle *handle = GetHandle()){
        //bad model.
        //handle->UpdateSampleType(EPolygonSampleType::EMeshedPolygonTrajectoryRayModel);
        
        //unet model - good
        handle->UpdateSampleType(EPolygonSampleType::EMeshedPolygonTrajectoryLayered);
    }

    //debug
    NNPluginSettings::LogSetting();
}


void ANNPathFinderSocket::Tick(float deltatime){
    //write NN type updates from Request Handle (for now)
    UpdateSelectedModelToSharedMemory();

    NNRequestHandle *handle = GetHandle();
    FString hasHandle = handle != nullptr ? "Has Handle" : "No Handle";
    DebugHelper::logMessage(FString::Printf(TEXT("ANNPathFinderSocket::Tick %s"), *hasHandle));

    Super::Tick(deltatime);
}

void ANNPathFinderSocket::UpdateSelectedModelToSharedMemory(){

    // --- todo: only on change model ---

    if(NNRequestHandle *handle = GetHandle()){
        EPolygonSampleType type = handle->GetSampleType();

        //write to buffer / shared memory
        TArray<uint8> data;
        EPolygonSampleTypeNameConversion::WriteNNTypeIntTo(type, data);
        WriteData(frameNameNNType, data);
    }
}



void ANNPathFinderSocket::EnableHeatMapSaveOnEnd(bool saveOnEnd){
    if(NNRequestHandle *handle = GetHandle()){
        handle->EnableHeatMapSaveOnEnd(saveOnEnd);
    }
}


bool ANNPathFinderSocket::HeatMapSaveOnEndEnabled(){
    if(NNRequestHandle *handle = GetHandle()){
        return handle->HeatMapSaveOnEndEnabled();
    }
    return false;
}

void ANNPathFinderSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---
    //TickTask();

    if(logConnection){
        DebugHelper::showScreenMessage("ANNPathFinderSocket::TickSocketConnected", FColor::Cyan);
    }

    
}











//// ---- interaction ----


//flags a aactor as spotted
void ANNPathFinderSocket::FlagVisible(AActor *actor){
    if(NNRequestHandle *handle = GetHandle()){
        handle->FlagVisible(actor);
    }
}


// ---- subrscribe observer pattern ----
void ANNPathFinderSocket::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    if(actor){
        FString name = actor->GetName();
        if(requestHandle){
            requestHandle->PredictData(interfaceNotify, name, actor);
        }
    }
}



HeatMapReceivers &ANNPathFinderSocket::GetHeatMapReceivers(){
    if(NNRequestHandle *handle = GetHandle()){
        return handle->GetHeatMapReceivers();
    }else{
        DebugHelper::logMessage("ANNPathFinderSocket::Could Not Find HeatMapReceivers!");
    }
    return empty;
}