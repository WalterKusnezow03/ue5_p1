#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "p2/_world/PlayerStatManager/PlayerStatus.h"

#include "HudUiActor.generated.h"

UCLASS()
class P2_API AHudUiActor : public ACustomMeshUIActor {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprintpath")
	FString blueprintPath;

public:
    static AHudUiActor *MakeInstance(
        UWorld *world, 
        USceneComponent *attachTo,
        FVector relativeLocation
    );

    void AttachToScene(USceneComponent *camera);


    // update

    void updateAmmunitionText(FString message);
    void updateHealthText(FString health);

    void updateTopWaringElement(FString message);
    void updateTopWarningElementTimed(FString message, float timetoLive);

    void Update(FPlayerStatus &playerStatusStruct);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

private:


};