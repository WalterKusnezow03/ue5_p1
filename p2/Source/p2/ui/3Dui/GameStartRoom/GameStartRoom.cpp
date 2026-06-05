#include "GameStartRoom.h"
#include "DebugPlugin/DebugHelper.h"
#include "AssetEnumCollection/assetEnums/EGameActorEnum.h"
#include "p2/ui/3Dui/GameStartRoom/WorldList/WorldListWidget.h"
#include "p2/ui/3Dui/GameStartRoom/WorldCreator/WorldCreatorWidget.h"
#include "p2/ui/3Dui/GameStartRoom/NNTrainWidget/NNTrainWidget.h"
#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"
#include "p2/_world/worldLevel.h"

AGameStartRoom *AGameStartRoom::instance = nullptr;

AGameStartRoom::AGameStartRoom() : Super() {
    PrimaryActorTick.bCanEverTick = true; 
    PrimaryActorTick.bTickEvenWhenPaused = true; 
}


void AGameStartRoom::CreateInstanceIfNeeded(AActor *actor){
    if(instance){
        DebugHelper::logMessage("AGameStartRoom Already has instance");
        return;
    }
    if(actor){
        if(!instance){
            instance = TMakeInstance<AGameStartRoom>(
                actor->GetWorld(),
                EGameActorEnum::EGameStartRoom,
                FVector(-10000, -10000, -3000)
            );
        }
    }
}


void AGameStartRoom::EndPlay(const EEndPlayReason::Type EndPlayReason){
    
    //Leave();
    //ClearReferences();
    instance = nullptr;
    Super::EndPlay(EndPlayReason);
}

void AGameStartRoom::ClearReferencesOnEndPlay(){
    if(UWorldCreatorWidget *widget = GetWorldCreatorWidget()){
        widget->ResetParent();
    }
    if(UWorldListWidget *widget = GetWorldListWidget()){
        widget->ResetParent();
    }
    if(UNNTrainWidget *widget = GetNNWidget()){
        widget->ResetParent();
    }
}

void AGameStartRoom::SetParentReferencesOnWidgets(){
    if(UWorldCreatorWidget *widget = GetWorldCreatorWidget()){
        widget->SetParent(this);
    }
    if(UWorldListWidget *widget = GetWorldListWidget()){
        widget->SetParent(this);
    }
    if(UNNTrainWidget *widget = GetNNWidget()){
        widget->SetParent(this);
    }
}


void AGameStartRoom::BeginPlay(){
    Super::BeginPlay();
    FindActorsOnBeginPlay();
}


//for getting widgets from child actors
void AGameStartRoom::FindActorsOnBeginPlay(){
    uiActorWorldCreator = TFindChildActorByName<ACustomMeshUIActor>("WorldCreatorWidgetActorBP");
    uiActorWorldList = TFindChildActorByName<ACustomMeshUIActor>("WorldListWidgetActorBP");
    uiActorNNTrain = TFindChildActorByName<ACustomMeshUIActor>("NNTrainWidgetActorBP");

    SetParentReferencesOnWidgets();
}

//helper to get from inside of ACustomMeshUIActor
UWorldCreatorWidget *AGameStartRoom::GetWorldCreatorWidget(){
    if(uiActorWorldCreator){
        return uiActorWorldCreator->GetWidget<UWorldCreatorWidget>();
    }
    return nullptr;
}

UWorldListWidget *AGameStartRoom::GetWorldListWidget(){
    if(uiActorWorldList){
        return uiActorWorldList->GetWidget<UWorldListWidget>();
    }
    return nullptr;
}

UNNTrainWidget *AGameStartRoom::GetNNWidget(){
    if(uiActorNNTrain){
        return uiActorNNTrain->GetWidget<UNNTrainWidget>();
    }
    return nullptr;
}

void AGameStartRoom::Tick(float deltatime){
    Super::Tick(deltatime);
    ListenForUserInput();
    
}

void AGameStartRoom::ListenForUserInput(){
    if(playerEntered){
        //DebugHelper::showScreenMessage("AGameStartRoom::Tick A");
        
        
        //APlayerController *APlayerControllerBase::GetPlayerController()
        if(APlayerControllerBase *casted = Cast<APlayerControllerBase>(playerEntered)){
            //DebugHelper::showScreenMessage("AGameStartRoom::Tick B");
            //keyboard dispatch
            casted->CollectUserInput(input);

            
            //DebugHelper::showScreenMessage("AGameStartRoom::Tick C");
            if(input.HasAnyKeyboardInput()){
                //DebugHelper::showScreenMessage("AGameStartRoom::Tick D");
                // ---- DISPATCH to sub widgets ----

                
                if(UWorldListWidget *worldList = GetWorldListWidget()){
                    if(worldList->TextBoxIsActive()){
                        worldList->dispatchUserInput(input);
                    }
                }

                if(UWorldCreatorWidget *creatorWidget = GetWorldCreatorWidget()){
                    if(creatorWidget->TextBoxIsActive()){
                        creatorWidget->dispatchUserInput(input);
                    }
                }


            }
        }
    }
}





void AGameStartRoom::StaticEnter(AActor *actor){
    DebugHelper::logMessage("AGameStartRoom::StaticEnter - A");
    if (actor)
    {
        DebugHelper::logMessage("AGameStartRoom::StaticEnter - B");
        CreateInstanceIfNeeded(actor);
        if (instance)
        {
            instance->Enter(actor);
            DebugHelper::logMessage("AGameStartRoom::StaticEnter - C");
        }
    }
}



bool AGameStartRoom::Enter(AActor *player){
    if(Super::Enter(player)){
        if(playerEntered){
            playerEntered->SetActorLocation(GetActorLocation());
        }
        return true;
    }
    return false;
    /*
    if(playerEntered){
        return;
    }
    if(player){
        enteredLocation = player->GetActorLocation();
        playerEntered = player;
        playerEntered->SetActorLocation(GetActorLocation());
    }*/
}

void AGameStartRoom::Leave(){
    if(playerEntered){
        //save some pos from world on save!
        playerEntered->SetActorLocation(FVector(0, 0, 200)); 
        LockPlayerMovement(false);
        //playerEntered = nullptr;
    }
    Super::Leave();
}

void AGameStartRoom::TryCreateWorld(FString worldName){
    if(worldName.Len() > 0){
        
        if(UWorldListWidget *list = GetWorldListWidget()){

            //does not exist yet if added sucessfully
            if(list->AddNewItem(worldName)){
                LaunchWorld(worldName);
            }
        }
    }
}





void AGameStartRoom::LaunchWorld(FString worldName){
    if(worldName.Len() > 0){
        FString message = FString::Printf(TEXT("UGameLaunchScreen Launch World: %s"), *worldName);
        Leave();
        DebugHelper::logMessage(message);
        DebugHelper::showScreenMessage(message, FColor::Purple);

        AworldLevel::gameStateManager.UpdateGameState(EGameState::EGamePlay);
        AworldLevel::initWorld(worldName);
        
    }
}





void AGameStartRoom::ChangeLockPlayerMovementBasedOnSeletedTextField(){
    LockPlayerMovement(AnyTextFieldSelected());
}

bool AGameStartRoom::AnyTextFieldSelected(){
    if(UWorldListWidget *worldList = GetWorldListWidget()){
        if(worldList->TextBoxIsActive()){
            return true;
        }
    }

    if(UWorldCreatorWidget *creatorWidget = GetWorldCreatorWidget()){
        if(creatorWidget->TextBoxIsActive()){
            return true;
        }
    }
    return false;
}

void AGameStartRoom::NotifyOnClickDispatch(){
    ChangeLockPlayerMovementBasedOnSeletedTextField();
}



void AGameStartRoom::NotifyNNTrainLaunch(){

    //subscribe receiver
    if(ANNPathFinderSocket *socket = ANNPathFinderSocket::PathFinderNNinstance()){
        if(UNNTrainWidget *widget = GetNNWidget()){
            socket->SubscribeMessageListener(widget);
        }
        //notify nn train launch
        socket->SetTrainingAllowed();
        //DebugHelper::logMessage("AGameStartRoom::NotifyNNTrainLaunch");
    }
}