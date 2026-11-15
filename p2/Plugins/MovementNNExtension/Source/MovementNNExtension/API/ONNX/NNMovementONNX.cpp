#include "NNMovementONNX.h"

#include "NNCommunicationPlugin/GameNN/ModelInput/FONNXModelInput.h"
#include "MovementNNExtension/GameNN/FMovementONNXModelsetup.h"
#include "NNCommunicationPlugin/API/Handle/NNRequestHandleTickDataOnnx.h"

ANNMovementONNX *ANNMovementONNX::instancePtr = nullptr;

ANNMovementONNX::ANNMovementONNX() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

ANNMovementONNX *ANNMovementONNX::ApiInstance(){
    if(instancePtr){
        if(ANNMovementONNX *casted = Cast<ANNMovementONNX>(instancePtr)){
            return casted;
        }
    }
    return nullptr;
}

void ANNMovementONNX::MakeMovementONNXInstance(UWorld* World)
{
    if(ApiInstance()){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ANNPathFinderAPIActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ANNMovementONNX* Spawned = World->SpawnActor<ANNMovementONNX>(
            ANNMovementONNX::StaticClass(), Location, Rotation, SpawnParams
        );
        instancePtr = Spawned;
    }
}


void ANNMovementONNX::OnBeginPlaySetupHandleStructure(){
    requestHandle = new NNMovementRequestHandle();
    requestTickData = new FNNRequestHandleTickDataOnnx();

}

NNMovementRequestHandle *ANNMovementONNX::GetHandle(){
    if(requestHandle){
        return static_cast<NNMovementRequestHandle *>(requestHandle);
    }
    return nullptr;
}

void ANNMovementONNX::OnBeginPlayLoadModels(){
    FMovementONNXModelsetup setup;
    LoadModel(setup, modelName());
}

FString ANNMovementONNX::modelName(){
    return "MovementOnnx";
}

ONNXModel *ANNMovementONNX::FindSelectedModel(){
    return 
    ANNCommunicationONNX::FindSelectedModel(
        modelName()
    );
}



//flags a aactor as spotted


// ---- subrscribe observer pattern ----
void ANNMovementONNX::PredictTrajectories(
    INNMovementInterface *interfaceNotify,
    AActor *actor
){
    if(actor){
        FString name = actor->GetName();
        PredictData(interfaceNotify, name, actor);
    }
}


void ANNMovementONNX::EndPlay(const EEndPlayReason::Type EndPlayReason){
    if(instancePtr == this){
        instancePtr = nullptr;
    }
    Super::EndPlay(EndPlayReason);
}