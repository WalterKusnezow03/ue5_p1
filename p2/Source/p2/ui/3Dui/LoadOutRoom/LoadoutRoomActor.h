#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/WeaponTableActor.h"
#include "p2/ui/3Dui/LoadOutRoom/ExitActor/LoadoutRoomExitButtonActor.h"

#include "p2/ui/3Dui/RoomBase/WidgetRoomActorBase.h"

#include "LoadoutRoomActor.generated.h"

UCLASS()
class P2_API ALoadoutRoomActor : public AWidgetRoomActorBase {
    GENERATED_BODY()

protected:
    static ALoadoutRoomActor *instance;

public:
    

    ALoadoutRoomActor();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;

    virtual bool Enter(AActor *actor) override;
    virtual void Leave() override;

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
    
    

    void TriggerEnteredAnimation();
    void TriggerLeaveAnimation();
    

    
    LoadoutHelper loadout;

    UPROPERTY()
    TArray<AWeaponTableActor *> weaponTables;

    UPROPERTY()
    ALoadoutRoomExitButtonActor *exitButton = nullptr;

    virtual void ClearReferencesOnEndPlay() override;

    
};