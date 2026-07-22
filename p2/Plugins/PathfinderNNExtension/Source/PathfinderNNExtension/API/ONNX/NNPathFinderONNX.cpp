#include "NNPathFinderONNX.h"
#include "PathfinderNNExtension/Settings/NNSetting.h"

ANNPathFinderONNX *ANNPathFinderONNX::instancePtr = nullptr;


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


void ANNPathFinderONNX::BeginPlay(){
    Super::BeginPlay();
    
    model.LoadModel(); //auto loads on construct
    requestHandle.BeginPlay();


    //debug
    NNPluginSettings::LogSetting();
}


bool ANNPathFinderONNX::NNIsConnected(){
    return model.WasLoaded();
}


void ANNPathFinderONNX::Tick(float deltatime){
    Super::Tick(deltatime);

    if(logConnection){
        DebugHelper::showScreenMessage("ANNPathFinderONNX::Active", FColor::Cyan);
    }
    

    /*//prepare and execute task
    requestTickData.Reset();
    requestTickData.bBatchBinaryDataNeeded = false;
    requestTickData.deltatime = deltatime;
    requestHandle.Tick(requestTickData);

    //is tested!


    // -----> ADD ASYNC OPERATION SUPPORT HERE!!!
    // -----> MUTEX LOCK UNLOCK!


    //has new request
    if(requestTickData.bRequestBinaryOutChanged){
        //new request: immidiate call for in and out

        //read from tick data: request
        //write to tick data: result
        TArray<float> predictionGenerated;
        if(model.RunModel(
            requestTickData.requestDataOut, //TArray<uint8> &bufferIn, //binary like python / shared mem
            predictionGenerated //TArray<float> &bufferPredictionOut
        )){
            requestTickData.bHasPredictionData = true; 
            
            //Notify immidiate
            requestHandle.ReadDataResultImmidiate(predictionGenerated);
        }
    }*/


    //NEW
    if(PrepareRequestData(deltatime)){
        requestTickData.bIsBusy = true;
        bool result = model.RunModelAsync(requestTickData.requestDataOut, [this](TArray<float> predictionGenerated)
        {
            // DIESER BLOCK LÄUFT AUTOMATISCH WIEDER IM GAMETHREAD
            //Notify immidiate
            this->requestHandle.ReadDataResultImmidiate(predictionGenerated);
            this->requestTickData.bIsBusy = false;
        });
        if(!result){
            requestTickData.bIsBusy = false;
        }
    }
    
}

bool ANNPathFinderONNX::PrepareRequestData(float deltatime){
    if(requestTickData.bIsBusy){
        DebugHelper::showScreenMessage("ANNPathFinderONNX::BUSY", FColor::Orange);
        return false;
    }

    //prepare and execute task
    requestTickData.Reset();
    requestTickData.bBatchBinaryDataNeeded = false;
    requestTickData.deltatime = deltatime;
    requestHandle.Tick(requestTickData);
    if(requestTickData.bRequestBinaryOutChanged){
        return true;
    }
    return false;
}





void ANNPathFinderONNX::EndPlay(const EEndPlayReason::Type EndPlayReason){
    requestHandle.EndPlay();
    Super::EndPlay(EndPlayReason);
}



//flags a aactor as spotted
void ANNPathFinderONNX::FlagVisible(AActor *actor){
    requestHandle.FlagVisible(actor);
}


// ---- subrscribe observer pattern ----
void ANNPathFinderONNX::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    requestHandle.PredictNode(interfaceNotify, actor);
}
