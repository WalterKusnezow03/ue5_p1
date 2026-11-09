#include "NNPathFinderONNX.h"
#include "PathfinderNNExtension/Settings/NNSetting.h"
#include "PathfinderNNExtension/GameNN/FONNXModelsetup.h"
#include "NNCommunicationPlugin/GameNN/ModelInput/FONNXModelInput.h"




ANNPathFinderONNX::ANNPathFinderONNX() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

ANNPathFinderONNX *ANNPathFinderONNX::ApiInstance(){
    if(instancePtr){
        if(ANNPathFinderONNX *casted = Cast<ANNPathFinderONNX>(instancePtr)){
            return casted;
        }
    }
    
    return nullptr;
}

void ANNPathFinderONNX::MakePathFinderONNXInstance(UWorld* World)
{
    if(ApiInstance()){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ANNPathFinderAPIActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ANNPathFinderONNX* Spawned = World->SpawnActor<ANNPathFinderONNX>(
            ANNPathFinderONNX::StaticClass(), Location, Rotation, SpawnParams
        );
        instancePtr = Spawned;
    }
}


void ANNPathFinderONNX::OnBeginPlaySetupHandleStructure(){
    requestHandle = new NNRequestHandle();
    requestTickData = new FNNRequestHandleTickDataOnnx();

    if(NNRequestHandle *handle = GetHandle()){
        handle->UpdateSampleType(EPolygonSampleType::EMeshedPolygonTrajectoryLayered);
    }
}

NNRequestHandle *ANNPathFinderONNX::GetHandle(){
    if(requestHandle){
        return static_cast<NNRequestHandle *>(requestHandle);
    }
    return nullptr;
}

void ANNPathFinderONNX::OnBeginPlayLoadModels(){
    LoadModel(EPolygonSampleType::EMeshedPolygonTrajectoryLayered);
    LoadModel(EPolygonSampleType::EMeshedPolygonTrajectoryRayModel);
}

void ANNPathFinderONNX::BeginPlay(){
    Super::BeginPlay();
    //debug
    NNPluginSettings::LogSetting();
}



void ANNPathFinderONNX::LoadModel(EPolygonSampleType type){
    FString name = EPolygonSampleTypeNameConversion::ONNXFileName(type);
    FONNXModelsetup setup(type);
    ANNCommunicationONNX::LoadModel(setup, name);
}

ONNXModel *ANNPathFinderONNX::FindSelectedModel(){
    if(NNRequestHandle *handle = GetHandle()){
        EPolygonSampleType currentModel = handle->GetSampleType();
        
        //make String
        selectedModel = EPolygonSampleTypeNameConversion::ONNXFileName(currentModel);
        return ANNCommunicationONNX::FindSelectedModel(selectedModel);
    }
    return nullptr;
}











//flags a aactor as spotted
void ANNPathFinderONNX::FlagVisible(AActor *actor){
    if(NNRequestHandle *handle = GetHandle()){
        handle->FlagVisible(actor);
    }
}


// ---- subrscribe observer pattern ----
void ANNPathFinderONNX::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    if(actor){
        FString name = actor->GetName();
        PredictData(interfaceNotify, name, actor);
    }
}


void ANNPathFinderONNX::EnableHeatMapSaveOnEnd(bool saveOnEnd){
    //set auto to false (?)

    if(NNRequestHandle *handle = GetHandle()){
        //default
        handle->EnableHeatMapSaveOnEnd(saveOnEnd);
    }

    
}

bool ANNPathFinderONNX::HeatMapSaveOnEndEnabled(){
    if(NNRequestHandle *handle = GetHandle()){
        return handle->HeatMapSaveOnEndEnabled();
    }
    return false;
}




HeatMapReceivers &ANNPathFinderONNX::GetHeatMapReceivers(){
    if(NNRequestHandle *handle = GetHandle()){
        return handle->GetHeatMapReceivers();
    }
    return none;
}