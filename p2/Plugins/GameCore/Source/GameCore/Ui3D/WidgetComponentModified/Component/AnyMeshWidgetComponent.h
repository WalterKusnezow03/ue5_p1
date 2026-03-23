// Copyright Walter Kusnezow All Rights Reserved.
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
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"



#include "AnyMeshWidgetPlugin/Public/Component/FAnyMeshWidgetRayIntersectResult.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"

#include "AnyMeshWidgetPlugin/Public/Component/AnyMeshWidgetComponentBase.h"

//#include "GameCore/MeshGenBase/WidgetComponentModified/CopiedSource/WidgetComponentCustom.h"

#include "AnyMeshWidgetComponent.generated.h"

/// @brief Will allow to have ANY meshdata on the widget, still with proper hit
/// and uv / screenpos hits, to dispatch to the widget.
/// Allows any meshdata beyond default plane and cylinder !
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UAnyMeshWidgetComponent : public UAnyMeshWidgetComponentBase
//UWidgetComponentCustom
{
	GENERATED_BODY()
public:
    
    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction
    ) override;



    
    bool RayIntersect(
        const FVector &origin,
        const FVector &direction
    ) override;
    bool RayIntersectHover(
        const FVector &origin,
        const FVector &direction
    ) override;




    /// @brief will tell if this component is marked as hovered in
    /// UInteractionComponentHoveredCache -> needed for external use in customUi actor parent.
    bool IsMarkedHovered();

protected:
    

    virtual void CreateMaterial() override;

    //hit helpers
    FVector2D ToScreenUV(const FVector2D &other);
    FVector2D WidgetScreenPosition(
        FVector2D uv
    );

    IBaseUiInterface *GetWidgetAsIBaseUiInterface();


    //update hover state external on tick
    //update hover state if this widget is not hovered anymore. (reset widget hover state. Widget in 3D space left.)
    void UpdateHoverWidgetLeft();


    //update interact key hold on this widget
    void UpdateInteractKeyHoldState();
    bool isInteractHoldDown = false;



};