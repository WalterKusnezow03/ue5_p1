#pragma once

#include "CoreMinimal.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "p2/ui/3Dui/LoadOutRoom/ExitActor/widget/ExitButtonWidget.h"


#include "LoadoutRoomExitButtonActor.generated.h"


class ALoadoutRoomActor;
UCLASS()
class P2_API ALoadoutRoomExitButtonActor : public ACustomMeshUIActor {
    GENERATED_BODY()

    
public:

    //does allow 3D interaction.
    virtual bool AllowRayIntersectInteraction() override{
		return true;
	}
    
    void SetParentActor(ALoadoutRoomActor *parent);
    void ClearParentActor();
    void NotifyExit();

    UPROPERTY(EditAnywhere, Category = "WidgetScaleSetup")
    float scaleCmMeshDataTargeted = 200.0f;

protected:
    void ScaleMeshDataToDesiredScale();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;
    virtual void CreateWidgetMeshData() override;

    ALoadoutRoomActor *parentActor = nullptr;

    void SetupWidgetOnBeginPlay();
    UExitButtonWidget *GetInnerWidget();

private:


};