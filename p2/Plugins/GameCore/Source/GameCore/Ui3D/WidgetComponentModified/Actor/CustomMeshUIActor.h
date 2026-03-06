#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameCore/Ui3D/WidgetComponentModified/Component/AnyMeshWidgetComponent.h"
#include "GameCore/PlayerControllerBase/InteractionCallbackInterface/WidgetInteractionCallbackInterface.h"


#include "Blueprint/UserWidget.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "CustomMeshUIActor.generated.h"

/**
 * UI Actor that contains a Dynamic Mesh 3D Widget Component 
 */
UCLASS()
class GAMECORE_API ACustomMeshUIActor : public AActor 
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomMeshUIActor();

	// --- Collision & Interaction ---

	//will only execute if AllowRayIntersectInteraction() is true.
	bool RayIntersect(
        const FVector &origin,
        const FVector &direction
    );
	bool RayIntersectHover(
		const FVector &origin,
		const FVector &direction
	);
	
	virtual bool AllowRayIntersectInteraction(){
		return true;
	}

	void EnableCollision(bool flag);

	FString GetDebugName(){
		return debugUiActorName;
	}

	void ScaleMeshDataToMaxCm(float num);

	// ---- PLAYER CALLBACK FOR WIDGETS ----
	// ---- callback interface injection for p2 game widgets if needed ----
	// if a widget has processed work it can notify this interface via
	// this parent which will own a AnyMeshWidget and its UWidget.
	// (Do NOT break the hierachy of ACustomMeshUiActor->AnyMeshWidgetComp->UWidget)
	void SetCallbackForDelayedInteractions(IWidgetInteractionCallbackInterface *interfaceIn);
	
protected:
    void ResetWidgetInteractionCallbackIfNotHovered();
    IWidgetInteractionCallbackInterface *widgetInteracionCallbackInterface = nullptr;

	// ---- PLAYER CALLBACK FOR WIDGETS ---- END ----

protected:
	//property for debug
	UPROPERTY(EditAnywhere, Category = "Debug");
	FString debugUiActorName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetWidgetClassOnBeginPlay();

	//create your meshdata here, as expected default, no winding order flipping. 
	//winding order will be flipped inside scene proxy, but is
	//needed because widgets are otherwise black.
	virtual void CreateWidgetMeshData();
	bool bMeshDataCreated = false;

public:
	void SetDrawSize(FVector2D size);

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// 3D Widget Component that displays UI in the world space
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UAnyMeshWidgetComponent* Widget;
	
	// Set a new widget class for the widget component
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetWidgetClass(TSubclassOf<UUserWidget> NewWidgetClass);

	// Get the current widget class
	UFUNCTION(BlueprintPure, Category = "UI")
	TSubclassOf<UUserWidget> GetWidgetClass() const;

	// Generic template method to get the actual widget inside the widget component
	template<typename T>
	T* GetWidget() const
	{
		if (Widget && Widget->GetUserWidgetObject())
		{
			return Cast<T>(Widget->GetUserWidgetObject());
		}
		return nullptr;
	}

protected:
	// Current widget class reference - can be edited in the editor and will update the widget component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CurrentWidgetClass;
};