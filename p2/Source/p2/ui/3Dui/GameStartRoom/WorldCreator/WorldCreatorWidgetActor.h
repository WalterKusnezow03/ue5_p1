#pragma once

#include "CoreMinimal.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"


#include "WorldCreatorWidgetActor.generated.h"


UCLASS()
class P2_API AWorldCreatorWidgetActor : public ACustomMeshUIActor {
    GENERATED_BODY()

    
public:
    //does allow 3D interaction.
    virtual bool AllowRayIntersectInteraction() override{
		return true;
	}

    virtual void CreateWidgetMeshData() override;

};