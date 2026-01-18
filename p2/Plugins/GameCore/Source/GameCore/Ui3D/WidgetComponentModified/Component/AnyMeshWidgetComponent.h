#pragma once

#include "Components/WidgetComponent.h"

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "Layout/ArrangedWidget.h"
#include "WorldCollision.h"
#include "Components/MeshComponent.h"
#include "Blueprint/UserWidget.h"

#include "Engine/GameInstance.h"
#include "Materials/Material.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineGlobals.h"
#include "MaterialShared.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialRenderProxy.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SWindow.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Input/HittestGrid.h"
#include "SceneManagement.h"
#include "DynamicMeshBuilder.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/BodySetup.h"
#include "Slate/SGameLayerManager.h"
#include "Slate/WidgetRenderer.h"
#include "Slate/SWorldWidgetScreenLayer.h"
#include "UObject/EditorObjectVersion.h"
#include "Widgets/SViewport.h"
#include "SceneInterface.h"

//custom mesh data
#include "GameCore/MeshGenBase/MeshData/MeshData.h"


//#include "GameCore/MeshGenBase/WidgetComponentModified/CopiedSource/WidgetComponentCustom.h"

#include "AnyMeshWidgetComponent.generated.h"

/// @brief Will allow to have ANY meshdata on the widget, still with proper hit
/// and uv / screenpos hits, to dispatch to the widget.
/// Allows any meshdata beyond default plane and cylinder !
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UAnyMeshWidgetComponent : public UWidgetComponent
//UWidgetComponentCustom
{
	GENERATED_BODY()
public:
    UAnyMeshWidgetComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
    {
        SetGeometryMode(EWidgetGeometryMode::Plane); //to force custom meshdata
    }

    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction) override;

    //jeden frame, RENDER DATA ONLY
    virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

    //COLLISION DATA
    virtual UBodySetup* GetBodySetup() override;
    virtual FCollisionShape GetCollisionShape(float Inflation) const override;


    /** Ensures the 3d window is created its size and content. */
	virtual void UpdateWidget() override {
        Super::UpdateWidget();
    }

    void OverrideMeshData(MeshData &data);

    MeshData &GetMeshDataRef();

    bool RayIntersect(
        const FVector &origin,
        const FVector &direction
    );

protected:
    MeshData assignedMeshData;
    bool MeshDataWasModified = false;

    void FlagMeshDataDirty();

    void CreateMaterial();

    FVector2D ToScreenUV(const FVector2D &other);
    FVector2D WidgetScreenPosition(
        FVector2D uv
    );

    void UpdateBodySetupOverride(); //copied defintion from source
};