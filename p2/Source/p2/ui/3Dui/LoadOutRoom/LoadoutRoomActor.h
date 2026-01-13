#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/WeaponTableActor.h"

#include "LoadoutRoomActor.generated.h"

UCLASS()
class P2_API ALoadoutRoomActor : public AActorBase {
    GENERATED_BODY()

public:
    static ALoadoutRoomActor *MakeInstance(UWorld *world);

    ALoadoutRoomActor();

    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    void Enter(AActor *actor);
    void Leave();


    //methods for Widget Actor Tables to call


protected:
    void FindActorsOnBeginPlay();

    void UpdatePlayerInventory();
    void ResetPlayerLocation();

    void TriggerEnteredAnimation();
    void TriggerLeaveAnimation();
    
    
    FVector enteredLocation;
    AActor *playerEntered = nullptr;

    LoadoutHelper loadout;
    TArray<AWeaponTableActor *> weaponTables;
};