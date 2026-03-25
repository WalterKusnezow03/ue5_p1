#pragma once

#include "CoreMinimal.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/WeaponTableWidget.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"

#include "WeaponTableWidgetActor.generated.h"

class AWeaponTableActor;

UCLASS()
class P2_API AWeaponTableWidgetActor : public ACustomMeshUIActor {
    GENERATED_BODY()

    
public:
    static AWeaponTableWidgetActor *MakeInstance(
        UWorld *world, 
        USceneComponent *attachTo,
        FVector relativeLocation
    );

    void AttachToScene(USceneComponent *scene);

    //does allow 3D interaction.
    virtual bool AllowRayIntersectInteraction() override{
		return true;
	}

    void SetWeaponSetupHelperRefernce(weaponSetupHelper *setup);

    //notify from UWidget
    void NotifyWeaponSetupChange();

    void SetParentActor(AWeaponTableActor *parent);

    UPROPERTY(EditAnywhere, Category = "WidgetScaleSetup")
    float scaleCmMeshDataTargeted = 300.0f;

    void UpdateDebugNameFromIndex(int index);

protected:
    void ScaleMeshDataToDesiredScale();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;
    virtual void CreateWidgetMeshData() override;

    UWeaponTableWidget *GetInnerWidget();



    AWeaponTableActor *parentActor = nullptr;


private:


};