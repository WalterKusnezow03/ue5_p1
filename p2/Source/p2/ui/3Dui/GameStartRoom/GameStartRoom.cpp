#include "GameStartRoom.h"
#include "DebugPlugin/DebugHelper.h"
#include "AssetEnumCollection/assetEnums/EGameActorEnum.h"
#include "p2/ui/3Dui/GameStartRoom/WorldList/WorldListWidget.h"
#include "p2/ui/3Dui/GameStartRoom/WorldCreator/WorldCreatorWidget.h"
#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"

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
        CreateInstanceIfNeeded(actor->GetWorld());
    }
}

void AGameStartRoom::CreateInstanceIfNeeded(UWorld *world){
    if(instance){
        return;
    }

    if(world){
        //instance = MakeInstance(world); //not needed, is set in function
        MakeInstance(world);
    } 
}


AGameStartRoom *AGameStartRoom::MakeInstance(UWorld *world){
    
    if(instance){
        return instance;
    }

    UClass *SpawnClass = nullptr;
    if(assetManager *a = assetManager::instance()){
        SpawnClass = a->Find<EGameActorEnum, UClass>(EGameActorEnum::EGameStartRoom);
    }
    if(!SpawnClass){
        DebugHelper::logMessage("AGameStartRoom::MakeInstance - Spawn class not found");
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FVector Location(-10000, -10000, -3000);
    AGameStartRoom *spawned = world->SpawnActor<AGameStartRoom>(
        SpawnClass,
        Location,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (!spawned)
    {
        DebugHelper::logMessage("AGameStartRoom::MakeInstance - Spawn actor failed");
        return nullptr;
    }

    //save instance ptr, only one needed.
    instance = spawned;
    DebugHelper::logMessage("ALoadoutRoomActor made instance");
    return spawned;
}


void AGameStartRoom::EndPlay(const EEndPlayReason::Type EndPlayReason){
    
    Leave();
    ClearReferences();
    instance = nullptr;
    Super::EndPlay(EndPlayReason);
}

void AGameStartRoom::ClearReferences(){
    if(UWorldCreatorWidget *widget = GetWorldCreatorWidget()){
        widget->ResetParent();
    }
    if(UWorldListWidget *widget = GetWorldListWidget()){
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
}


void AGameStartRoom::BeginPlay(){
    Super::BeginPlay();
    FindActorsOnBeginPlay();
}


//for getting widgets from child actors
void AGameStartRoom::FindActorsOnBeginPlay(){
    uiActorWorldCreator = TFindChildActorByName<ACustomMeshUIActor>("WorldCreatorWidgetActorBP");
    uiActorWorldList = TFindChildActorByName<ACustomMeshUIActor>("WorldListWidgetActorBP");

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

void AGameStartRoom::Tick(float deltatime){
    Super::Tick(deltatime);
    ListenForUserInput();
    
}

void AGameStartRoom::ListenForUserInput(){
    if(playerEntered){
        DebugHelper::showScreenMessage("AGameStartRoom::Tick A");
        
        
        //APlayerController *APlayerControllerBase::GetPlayerController()
        if(APlayerControllerBase *casted = Cast<APlayerControllerBase>(playerEntered)){
            DebugHelper::showScreenMessage("AGameStartRoom::Tick B");
            //keyboard dispatch
            casted->CollectUserInput(input);

            
            DebugHelper::showScreenMessage("AGameStartRoom::Tick C");
            if(input.HasAnyKeyboardInput()){
                DebugHelper::showScreenMessage("AGameStartRoom::Tick D");
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
        if(!instance){
            instance = MakeInstance(actor->GetWorld());
        }
        if(instance){
            instance->Enter(actor);
            DebugHelper::logMessage("AGameStartRoom::StaticEnter - C");
        }
    }
}



void AGameStartRoom::Enter(AActor *player){
    if(playerEntered){
        return;
    }
    if(player){
        enteredLocation = player->GetActorLocation();
        playerEntered = player;
        playerEntered->SetActorLocation(GetActorLocation());
    }
}

void AGameStartRoom::Leave(){
    if(playerEntered){
        //save some pos from world on save!
        playerEntered->SetActorLocation(FVector(0, 0, 200)); 
        LockPlayerMovement(false);
        playerEntered = nullptr;
    }
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




void AGameStartRoom::LockPlayerMovement(bool flag){
    if(playerEntered){
        if(APlayerControllerBase *casted = Cast<APlayerControllerBase>(playerEntered)){
            casted->SetMovementLocked(flag);

            
            FString message = TEXT("AGameStartRoom::LockPlayerMovement ");
            message += flag ? TEXT(" locked ") : TEXT(" unlocked ");
            DebugHelper::logMessage(message);
        }
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