#include "NNCommunicationONNX.h"

#include "NNCommunicationPlugin/GameNN/ModelInput/FONNXModelInput.h"

ANNCommunicationONNX *ANNCommunicationONNX::instancePtr = nullptr;


ANNCommunicationONNX::ANNCommunicationONNX() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}


void ANNCommunicationONNX::OnBeginPlaySetupHandleStructure(){
    requestHandle = nullptr;
    requestTickData = nullptr;
}

void ANNCommunicationONNX::OnEndPlayCleanUpHandleStructure(){
    if (requestHandle){
        requestHandle->EndPlay();
    }

    delete requestHandle;
    requestHandle = nullptr;

    delete requestTickData;
    requestTickData = nullptr;
}


void ANNCommunicationONNX::BeginPlay(){
    OnBeginPlaySetupHandleStructure();
    Super::BeginPlay();

    OnBeginPlayLoadModels();
    if(requestHandle){
        requestHandle->BeginPlay();
    }

}

void ANNCommunicationONNX::OnBeginPlayLoadModels(){
    //modelTrajectoryLayered.LoadModel(FONNXModelsetup(EPolygonSampleType::EMeshedPolygonTrajectoryLayered));
    //rayModel.LoadModel(FONNXModelsetup(EPolygonSampleType::EMeshedPolygonTrajectoryRayModel));    
}


void ANNCommunicationONNX::LoadModel(FONNXModelsetupBase &setup, FString name){
    models.erase(name);
    auto model = models.try_emplace(name).first;
    model->second.LoadModel(setup);
}

ONNXModel *ANNCommunicationONNX::FindSelectedModel(){
    return FindSelectedModel(selectedModel);
}

ONNXModel *ANNCommunicationONNX::FindSelectedModel(FString name){
    auto model = models.find(name);
    if(model != models.end()){
        return &model->second;
    }
    return nullptr;
}

bool ANNCommunicationONNX::NNIsConnected(){
    if(ONNXModel *model = FindSelectedModel()){
        return model->WasLoaded();
    }
    return false;
    // return model.WasLoaded();
}


void ANNCommunicationONNX::Tick(float deltatime){
    Super::Tick(deltatime);

    if(logConnection){
        DebugHelper::showScreenMessage("ANNPathFinderONNX::Active", FColor::Cyan);
    }

    if(!requestTickData){
        return;
    }
    if(!requestHandle){
        return;
    }

    if(ONNXModel *model = FindSelectedModel()){
        //NEW
        if(PrepareRequestData(deltatime)){
            requestTickData->bIsBusy = true;

            bool result = model->RunModelAsync(requestTickData->requestDataOut, [this](TArray<float> predictionGenerated)
            {
                // DIESER BLOCK LÄUFT AUTOMATISCH WIEDER IM GAMETHREAD
                //Notify immidiate
                this->requestHandle->ReadDataResultImmidiate(predictionGenerated);
                this->requestTickData->bIsBusy = false;
            });
            if(!result){
                requestTickData->bIsBusy = false;
            }
        }
    }



    
    
}

bool ANNCommunicationONNX::PrepareRequestData(float deltatime){
    if(!requestTickData){
        return false;
    }
    if(requestTickData->bIsBusy){
        DebugHelper::showScreenMessage("ANNPathFinderONNX::BUSY", FColor::Orange);
        return false;
    }

    //prepare and execute task
    requestTickData->Reset();
    requestTickData->bBatchBinaryDataNeeded = false;
    requestTickData->deltatime = deltatime;
    requestHandle->Tick(*requestTickData);
    if(requestTickData->bRequestBinaryOutChanged){
        return true;
    }
    return false;
}





void ANNCommunicationONNX::EndPlay(const EEndPlayReason::Type EndPlayReason){
    OnEndPlayCleanUpHandleStructure();
    if(instancePtr == this){
        instancePtr = nullptr;
    }
    
    Super::EndPlay(EndPlayReason);
}




