#pragma once

#include "CoreMinimal.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"


#include "WorldListWidgetActor.generated.h"


UCLASS()
class P2_API AWorldListWidgetActor : public ACustomMeshUIActor {
    GENERATED_BODY()

    
public:
    //does allow 3D interaction.
    virtual bool AllowRayIntersectInteraction() override{
		return true;
	}

    virtual void CreateWidgetMeshData() override;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};