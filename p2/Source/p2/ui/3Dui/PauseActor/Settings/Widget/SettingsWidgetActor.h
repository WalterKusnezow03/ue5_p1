#pragma once

#include "CoreMinimal.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"


#include "SettingsWidgetActor.generated.h"


UCLASS()
class P2_API ASettingsWidgetActor : public ACustomMeshUIActor {
    GENERATED_BODY()

    
public:
    ASettingsWidgetActor();

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