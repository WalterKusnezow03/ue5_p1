#pragma once

#include "CoreMinimal.h"

#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"

#include "MeshExtractedActor.generated.h"


///--- DEBUG CLASS ---

UCLASS()
class MESHDATAPLUGIN_API AMeshExtractedActor : public ACustomMeshUIActor{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AMeshExtractedActorSettings")
    FString meshExteriorName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AMeshExtractedActorSettings")
    FString meshPlaneName;

protected:
    virtual void BeginPlay() override;
    void ExtractMeshDataOnBeginPlay();
    virtual void Tick(float deltatime) override;

    bool loadedMaterials = false;
    materialEnum matType = materialEnum::redsandMaterial;
};