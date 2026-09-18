
#pragma once

#include "CoreMinimal.h"

#include "p2/entetiesBase/miniMapRegisteredActor/MiniMapRegisteredActor.h"
#include "PathfinderNNExtension/Interface/PathfinderNNHeatMapReceiver.h"

#include "p2/renderTargets/customRenderTarget/CustomRenderedTexture.h"
#include "p2/renderTargets/RenderTargetActorBase/RenderTargetActorBase.h"

#include "NNHeatMapActor.generated.h"


// will not be registered to minimap 
// and will provide custom image data
// for the displayed widget

UCLASS()
class P2_API ANNHeatMapActor : 
public ARenderTargetActorBase,
public IPathfinderNNHeatMapReceiver
{
	GENERATED_BODY()

    //todo - scene root needed!

private:
    Image widgetImageData;

public:
    ANNHeatMapActor();

    static void MakeInstance(UWorld *world);
    static void MakeInstanceAttachTo(AActor *actor, FVector offset);
    static void MakeInstanceAttachTo(USceneComponent *TargetComponent, FVector Offset);

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



    //interface
    virtual void ResponseHeatMap(const Image &image) override {
        //works as expected
        //DebugHelper::logMessage("ANNHeatMapActor::Receive Image");
        widgetImageData.Copy(image);
    }

    virtual void SetLocation(const FVector &location) override {
        //deprecated.

        //SetActorLocation(location);
        //FVector update = GetActorLocation();

        //works aswell
        /*
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("ANNHeatMapActor::Update location (%.2f %.2f) (%.2f %.2f)"),
                location.X, 
                location.Y,
                update.X,
                update.Y
            )
        );*/
    }
    //interface
};