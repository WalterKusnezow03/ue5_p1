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
    for(UNNTrainWidget *widget : GetNNWidgets()){
        if(widget){
            widget->ResetParent();
        }
    }
    
}

void AGameStartRoom::SetParentReferencesOnWidgets(){
    if(UWorldCreatorWidget *widget = GetWorldCreatorWidget()){
        widget->SetParent(this);
    }
    if(UWorldListWidget *widget = GetWorldListWidget()){
        widget->SetParent(this);
    }
    for(UNNTrainWidget *widget : GetNNWidgets()){
        if(widget){
            widget->SetParent(this);
        }
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
    
    uiActorNNTrain_PathFinder = TFindChildActorByName<ACustomMeshUIActor>("NNTrainWidgetActor_Pathfinder");
    uiActorNNTrain_Movement = TFindChildActorByName<ACustomMeshUIActor>("NNTrainWidgetActor_Movement");


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

UNNTrainWidget *AGameStartRoom::GetNNWidget(FString target){
    //match name must be exact
    if(Match(target, targetPlugin_NNPathFinder)){
        DebugHelper::logMessage(FString::Printf(TEXT("AGameStartRoom::GetNNWidget pathfinder %s match %s"), *target, *targetPlugin_NNPathFinder));
        if(uiActorNNTrain_PathFinder){
            return uiActorNNTrain_PathFinder->GetWidget<UNNTrainWidget>();
        }
    }
    if(Match(target, targetPlugin_NNMovement)){
    //if(target.ToLower().Equals(targetPlugin_NNMovement.ToLower())){
        DebugHelper::logMessage(FString::Printf(TEXT("AGameStartRoom::GetNNWidget movement %s match %s"), *target, *targetPlugin_NNMovement));
        if(uiActorNNTrain_Movement){
            return uiActorNNTrain_Movement->GetWidget<UNNTrainWidget>();
        }
    }
    return nullptr;
}

bool AGameStartRoom::Match(FString targetPlugin, FString comparePlugin){
    return targetPlugin.ToLower().Equals(comparePlugin.ToLower());
}

TArray<UNNTrainWidget *> AGameStartRoom::GetNNWidgets(){
    TArray<UNNTrainWidget *> outArray;
    if(uiActorNNTrain_PathFinder){
        outArray.Add(uiActorNNTrain_PathFinder->GetWidget<UNNTrainWidget>());
    }
    if(uiActorNNTrain_Movement){
        outArray.Add(uiActorNNTrain_Movement->GetWidget<UNNTrainWidget>());
    }
    return outArray;
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


#include "PathfinderNNExtension/API/PublicInterface/NNPathFinderExtensionApi.h"
void AGameStartRoom::NotifyNNTrainLaunch(FString targetPlugin){
    
    DebugHelper::logMessage("GameStartRoom::NNPathFinderExtensionApi::SetTrainingAllowed ", targetPlugin);
    if(UNNTrainWidget *widget = GetNNWidget(targetPlugin)){
        //find matching plugin static api.
        if(Match(targetPlugin, targetPlugin_NNPathFinder)){
            NNPathFinderExtensionApi::SubscribeMessageListener(widget);
            NNPathFinderExtensionApi::SetTrainingAllowed();
            DebugHelper::logMessage("GameStartRoom::NNPathFinderExtensionApi::SetTrainingAllowed");
        }
        if(Match(targetPlugin, targetPlugin_NNMovement)){
            NNMovementApi::SubscribeMessageListener(widget);
            NNMovementApi::SetTrainingAllowed();
            DebugHelper::logMessage("GameStartRoom::NNMovementApi::SetTrainingAllowed");
        }
    }

        
    


    /*UPROPERTY(EditAnywhere, Category = "NN|PluginName")
    FString targetPlugin_NNPathFinder = "PATHFINDERNNEXTENSION";

    UPROPERTY(EditAnywhere, Category = "NN|PluginName")
    FString targetPlugin_NNMovement = "MOVEMENTNNEXTENSION";*/


    
   
}