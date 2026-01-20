#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/WeaponTableActor.h"
#include "p2/ui/3Dui/LoadOutRoom/ExitActor/LoadoutRoomExitButtonActor.h"

#include "LoadoutRoomActor.generated.h"

UCLASS()
class P2_API ALoadoutRoomActor : public AActorBase {
    GENERATED_BODY()

protected:
    static ALoadoutRoomActor *instance;

public:
    static ALoadoutRoomActor *MakeInstance(UWorld *world);

    ALoadoutRoomActor();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;

    void Enter(AActor *actor);
    void Leave();

    static void StaticEnter(AActor *actor);
    static void StaticLeave();

    //methods for Widget Actor Tables to call


protected:
    static void CreateInstanceIfNeeded(AActor *actor);
    static void CreateInstanceIfNeeded(UWorld *world);

    void FindActorsOnBeginPlay();
    void ProcessFoundActors(TArray<AActor *> &actors);
    void UpdateLoadoutWithTableActors();

    void UpdatePlayerInventory();
    void ResetPlayerLocation();
    

    void TriggerEnteredAnimation();
    void TriggerLeaveAnimation();
    

    FVector enteredLocation;
    AActor *playerEntered = nullptr;
    
    LoadoutHelper loadout;

    UPROPERTY()
    TArray<AWeaponTableActor *> weaponTables;

    UPROPERTY()
    ALoadoutRoomExitButtonActor *exitButton = nullptr;

    void ClearReferences();

    //debug
    void logMessage(FString message);
    void logMessage(FString message, FVector pos);
    void showScreenMessage(FString message, FColor color);
};