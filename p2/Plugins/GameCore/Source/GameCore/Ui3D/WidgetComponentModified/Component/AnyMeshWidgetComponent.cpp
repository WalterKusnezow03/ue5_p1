#include "AnyMeshWidgetComponent.h"


void UAnyMeshWidgetComponent::BeginPlay(){
	Super::BeginPlay();

	DebugHelper::logMessage("UAnyMeshWidgetComponent::BeginPlay");
	
	//Material insance is handeld by base class begin play. None needed here!
	//although having none causes instant crash.
	/*if(assetManager *a = assetManager::instance()){
		DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial");
		UMaterialInterface *BaseMat = a->Find<materialEnum, UMaterial>(materialEnum::widgetMaterial);
		if (BaseMat){
			DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial Success");
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
		}
	}*/

	//WidgetComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel
	SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel
}

void UAnyMeshWidgetComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IBaseUiInterface *casted = GetWidgetAsIBaseUiInterface()){
		casted->Tick(DeltaTime);
	}
	UpdateHoverWidgetLeft();
	UpdateInteractKeyHoldState();
}

void UAnyMeshWidgetComponent::UpdateHoverWidgetLeft(){
	if(IBaseUiInterface *casted = GetWidgetAsIBaseUiInterface()){
		if(!IsMarkedHovered()){
			casted->removeHover();
		}
	}
}

bool UAnyMeshWidgetComponent::IsMarkedHovered(){
	return UInteractionComponentHoveredCache::IsHoveredWidgetComponent(this);
}





bool UAnyMeshWidgetComponent::RayIntersect(
    const FVector &origin,
    const FVector &direction
){	
	FAnyMeshWidgetRayIntersectResult result = RayIntersectResult(origin, direction);
	if(result.IsResultValid()){
		if(IBaseUiInterface *casted = GetWidgetAsIBaseUiInterface()){
			casted->dispatchClick(result.constScreenPositionReference());
		}
		return true;
	}
	return false;
}

bool UAnyMeshWidgetComponent::RayIntersectHover(
	const FVector &origin,
	const FVector &direction
){
	FAnyMeshWidgetRayIntersectResult result = RayIntersectResult(origin, direction);
	if(result.IsResultValid()){
		/*DebugHelper::showScreenMessage(
			"UAnyMeshWidgetComponent::RayIntersectHover VALID", 
			FColor::Yellow
		);*/

		//cast or click dispatcher
		if(IBaseUiInterface *casted = GetWidgetAsIBaseUiInterface()){
			casted->dispatchHover(result.constScreenPositionReference());	
		}
		//update hover widget cache, set this widget as hovered.
		UInteractionComponentHoveredCache::UpdateHovered(this);

		return true;
	}
	return false;
}

IBaseUiInterface* UAnyMeshWidgetComponent::GetWidgetAsIBaseUiInterface(){
	if(UWidget *w = GetWidget()){
		//cast or click dispatcher
		if(IBaseUiInterface *casted = Cast<IBaseUiInterface>(w)){
			return casted;
		}
	}
	return nullptr;
}





void UAnyMeshWidgetComponent::UpdateInteractKeyHoldState(){
	isInteractHoldDown = UInteractionComponentHoveredCache::IsInteractKeyHoldWidgetComponent(this);
	if(IBaseUiInterface *casted = GetWidgetAsIBaseUiInterface()){
		casted->UpdateInteractKeyHoldState(isInteractHoldDown);
	}
}