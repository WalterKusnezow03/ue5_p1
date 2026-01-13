#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/weapon/weapon.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/WeaponTableWidgetActor.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"

#include "WeaponTableActor.generated.h"

//spawns widget on its own
UCLASS()
class P2_API AWeaponTableActor : public ACustomMeshUIActor {
    GENERATED_BODY()

public:
    AWeaponTableActor();
    

    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    //update triggered from <- WidgetTableWidgetActor <- WeaponTableWidget
    void NotifyWeaponSetupChange();

    //must be called from loadoutroom on left room
    void UpdateLoadoutWithInternalSetup(LoadoutHelper &ref, int index);

protected:
    void SpawnWidgets();
    void SpawnWidgetOnBeginPlay();



    int tableIndex = 0;
    weaponSetupHelper setupHelper;
    Aweapon *weaponSpawned = nullptr;

    AWeaponTableWidgetActor *tableWidgetActor = nullptr;

    //not known if has to be here 
    //Aweapon *attachedWeaponForPreview = nullptr;
};