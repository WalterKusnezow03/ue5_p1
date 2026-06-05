#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/WeaponTableActor.h"
#include "p2/ui/3Dui/LoadOutRoom/ExitActor/LoadoutRoomExitButtonActor.h"
#include "customuipluginbase/UserInputTracking/UserInput.h"

#include "p2/ui/3Dui/RoomBase/WidgetRoomActorBase.h"

#include "GameStartRoom.generated.h"

class UWorldCreatorWidget;
class UWorldListWidget;
class UNNTrainWidget;

UCLASS()
class P2_API AGameStartRoom : public AWidgetRoomActorBase {
    GENERATED_BODY()

protected:
    static AGameStartRoom *instance;

    

public:
    
    AGameStartRoom();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;

    virtual bool Enter(AActor *actor) override;
    virtual void Leave() override;

    static void StaticEnter(AActor *actor);
    

    //methods for Widget Actor Tables to call

    void TryCreateWorld(FString worldName);
    void LaunchWorld(FString worldName);

    void NotifyOnClickDispatch();

    void NotifyNNTrainLaunch();

protected:
    static void CreateInstanceIfNeeded(AActor *actor);
    static void CreateInstanceIfNeeded(UWorld *world);

    void FindActorsOnBeginPlay();
    void ProcessFoundActors(TArray<AActor *> &actors);

    void ListenForUserInput();

    void SetParentReferencesOnWidgets();
    virtual void ClearReferencesOnEndPlay() override;

    UserInput input;


    //helper to get from inside of ACustomMeshUIActor
    UWorldCreatorWidget *GetWorldCreatorWidget();
    UWorldListWidget *GetWorldListWidget();
    UNNTrainWidget *GetNNWidget();

    //saved as raw ui actor to prevent additional subclasses
    ACustomMeshUIActor *uiActorWorldCreator = nullptr;
    ACustomMeshUIActor *uiActorWorldList = nullptr;
    ACustomMeshUIActor *uiActorNNTrain = nullptr;

    bool AnyTextFieldSelected();
    void ChangeLockPlayerMovementBasedOnSeletedTextField();
};