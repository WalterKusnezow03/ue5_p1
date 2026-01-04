#pragma once

#include "CoreMinimal.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "p2/_world/PlayerStatManager/PlayerStatus.h"

#include "p2/ui/3Dui/Widget/HudUiWidget.h"

#include "HudUiActor.generated.h"

UCLASS()
class P2_API AHudUiActor : public ACustomMeshUIActor {
    GENERATED_BODY()

protected:
    static AHudUiActor *instance;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprintpath")
	FString blueprintPath;

public:
    static AHudUiActor *MakeInstance(
        UWorld *world, 
        USceneComponent *attachTo,
        FVector relativeLocation
    );

    static AHudUiActor *GetInstance(){
        return instance;
    }

    void AttachToScene(USceneComponent *camera);


    // update
    void Update(FPlayerStatus &playerStatusStruct);

    //doesnt allow 3D interaction.
    virtual bool AllowRayIntersectInteraction() override{
		return false;
	}

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;
    virtual void CreateWidgetMeshData() override;

    UHudUiWidget *GetHudWidget();

private:


};