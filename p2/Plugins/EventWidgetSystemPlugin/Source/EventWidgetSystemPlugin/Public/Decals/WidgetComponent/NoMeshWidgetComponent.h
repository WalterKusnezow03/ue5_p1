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

#include "Components/DecalComponent.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventDispatcher/WidgetIdKey.h"

#include "NoMeshWidgetComponent.generated.h"

class UWidgetDecalComponent;

/// ----- BASE CLASS FOR NO MESH : DECAL UWIDGETS, without a mesh but PROJECTED UWIDGETS -----
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class EVENTWIDGETSYSTEMPLUGIN_API UNoMeshWidgetComponent : public UWidgetComponent
//UWidgetComponentCustom
{
	GENERATED_BODY()
public:
    UNoMeshWidgetComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
    {
        SetGeometryMode(EWidgetGeometryMode::Plane); //to force custom meshdata
    }

private:

    // -- id for event dispatcher --
    UPROPERTY(EditAnywhere, Category = "_EventOption")
    FString widgetName = "HeckWidget";

    UPROPERTY(EditAnywhere, Category = "_EventOption")
    FString actorName = "Actor47";
    // -- id for event dispatcher --

public:



    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction) override;


    template<typename T>
	T* TGetWidget() const{
		if (GetUserWidgetObject())
		{
			return Cast<T>(GetUserWidgetObject());
		}
		return nullptr;
	}
    
    void SetResolution(FVector2D res);

    

protected:
    /*virtual FPrimitiveSceneProxy* CreateSceneProxy() override {
        return nullptr;
    }*/

    void RegisterWidgetToEventDispatcherOnBeginPlay();
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void UnRegisterWidgetToEventDispatcherOnEndPlay();

    WidgetIdKey MakeKey();

    bool materialUpdated = false;

    UPROPERTY(EditAnywhere, Category = "_MaterialOption")
    TSoftObjectPtr<UMaterialInterface> decalBaseMaterialRef;

    bool materialWidgetInitied = false;
    UPROPERTY(EditAnywhere, Category = "_MaterialOption")
    TSoftObjectPtr<UMaterialInterface> widgetMaterialInvisible;
    void UpdateWidgetMaterialCustom();


    UDecalComponent *decal = nullptr;
    void UpdateDecalMaterial();

    UDecalComponent *FindDecal();
    void SetResolutionFromComponent();
};