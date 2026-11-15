#include "NNMovementSocket.h"
#include "DebugPlugin/DebugHelper.h"

ANNMovementSocket* ANNMovementSocket::movementIntancePtr = nullptr;

void ANNMovementSocket::MakeNNMovementSocketInstance(UWorld* World)
{
    if(NNMovementinstance()){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ANNMovementSocketActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ANNMovementSocket* Spawned = World->SpawnActor<ANNMovementSocket>(
            ANNMovementSocket::StaticClass(), Location, Rotation, SpawnParams
        );   
        movementIntancePtr = Spawned;
    }
}

void ANNMovementSocket::OnBeginPlaySetupHandleStructure(){
    requestHandle = new NNMovementRequestHandle();
    requestTickData = new FNNRequestHandleTickDataBase();

    
}

void ANNMovementSocket::OnBeginPlaySetupFrameNames(){
    //just as used with python process!
    //ANNPathFinderSF means ANNPathFinderSharedFrame
    frameNameRequest = "ANNMovementSFIN";
    frameNameResult = "ANNMovementSFRES";
    frameNameGroundThruth = "ANNMovementSFGT";
    frameNameBatch = "ANNMovementSFB";
    frameNameNNType = "ANNMovementSFNN";
}

void ANNMovementSocket::OnBeginPlayLaunchPython(){
    LaunchPythonProcess(
        "PathFinderNNExtension", 
        "nnpathfinder_server.py", //local nnpathfinder_server.py, from this plugin
        "NNCommunicationPlugin", //hier ist python installiert.
        "venv/bin/python"
    ); 
}

NNMovementRequestHandle *ANNMovementSocket::GetHandle(){
    if(requestHandle){
        return static_cast<NNMovementRequestHandle *>(requestHandle);
    }
    return nullptr;
}

void ANNMovementSocket::BeginPlay(){
    Super::BeginPlay();
}



void ANNMovementSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---
    //TickTask();

    if(logConnection){
        DebugHelper::showScreenMessage("ANNMovementSocket::TickSocketConnected", FColor::Cyan);
    }
    
    
}











//// ---- interaction ----




// ---- subrscribe observer pattern ----
void ANNMovementSocket::PredictTrajectories(
    INNMovementInterface *interfaceNotify,
    AActor *actor
){
    if(actor){
        FString name = actor->GetName();
        if(requestHandle){
            requestHandle->PredictData(interfaceNotify, name, actor);
        }
    }
}




void ANNMovementSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    if(movementIntancePtr == this){
        movementIntancePtr = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}