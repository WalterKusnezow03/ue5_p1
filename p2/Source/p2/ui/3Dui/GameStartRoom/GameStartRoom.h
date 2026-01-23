#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/WeaponTableActor.h"
#include "p2/ui/3Dui/LoadOutRoom/ExitActor/LoadoutRoomExitButtonActor.h"
#include "customuipluginbase/UserInputTracking/UserInput.h"

#include "GameStartRoom.generated.h"

class UWorldCreatorWidget;
class UWorldListWidget;

UCLASS()
class P2_API AGameStartRoom : public AActorBase {
    GENERATED_BODY()

protected:
    static AGameStartRoom *instance;

    

public:
    static AGameStartRoom *MakeInstance(UWorld *world);

    AGameStartRoom();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;

    void Enter(AActor *actor);
    void Leave();

    static void StaticEnter(AActor *actor);
    

    //methods for Widget Actor Tables to call

    void TryCreateWorld(FString worldName);
    void LaunchWorld(FString worldName);

    void NotifyOnClickDispatch();

protected:
    static void CreateInstanceIfNeeded(AActor *actor);
    static void CreateInstanceIfNeeded(UWorld *world);

    void FindActorsOnBeginPlay();
    void ProcessFoundActors(TArray<AActor *> &actors);

    void ListenForUserInput();

    FVector enteredLocation;
    AActor *playerEntered = nullptr;

    void SetParentReferencesOnWidgets();
    void ClearReferences();

    UserInput input;


    //helper to get from inside of ACustomMeshUIActor
    UWorldCreatorWidget *GetWorldCreatorWidget();
    UWorldListWidget *GetWorldListWidget();

    //saved as raw ui actor to prevent additional subclasses
    ACustomMeshUIActor *uiActorWorldCreator = nullptr;
    ACustomMeshUIActor *uiActorWorldList = nullptr;

    void LockPlayerMovement(bool flag);
    bool AnyTextFieldSelected();
    void ChangeLockPlayerMovementBasedOnSeletedTextField();
};