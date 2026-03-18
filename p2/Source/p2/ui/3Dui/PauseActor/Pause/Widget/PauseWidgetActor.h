#pragma once

#include "CoreMinimal.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/WeaponTableWidget.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"

#include "PauseWidgetActor.generated.h"


UCLASS()
class P2_API APauseWidgetActor : public ACustomMeshUIActor {
    GENERATED_BODY()

    
public:
    APauseWidgetActor();

    // does allow 3D interaction.
    virtual bool AllowRayIntersectInteraction() override{
		return true;
	}

    UPROPERTY(EditAnywhere, Category = "WidgetScaleSetup")
    float scaleCmMeshDataTargeted = 300.0f;

protected:
    void ScaleMeshDataToDesiredScale();

    virtual void CreateWidgetMeshData() override;

};