#include "AnyMeshWidgetComponent.h"

#include "DynamicMeshWidgetSceneProxy.h"
#include "GameCore/MeshGenBase/MeshData/intersectCache/IntersectHitResult.h"
#include "GameCore/Ui3D/InteractionComponentCache/InteractionComponentHoveredCache.h"

/*
UAnyMeshWidgetComponent::UAnyMeshWidgetComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {

      };*/

void UAnyMeshWidgetComponent::BeginPlay(){
	Super::BeginPlay();

	DebugHelper::logMessage("UAnyMeshWidgetComponent::BeginPlay");
	if(assetManager *a = assetManager::instance()){
		DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial");
		UMaterialInterface *BaseMat = a->Find<materialEnum, UMaterial>(materialEnum::widgetMaterial);
		if (BaseMat){
			DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial Success");
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
		}
	}

	//WidgetComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel
	SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel
}

void UAnyMeshWidgetComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(MeshDataWasModified){
		MarkRenderStateDirty(); //CreateSceneProxy will be called again.
	}


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

void UAnyMeshWidgetComponent::FlagMeshDataDirty(){
	MeshDataWasModified = true;
}


void UAnyMeshWidgetComponent::OverrideMeshData(MeshData &data){
	assignedMeshData = data;
	//MarkRenderStateDirty(); //calls CreateSceneProxy?
	FlagMeshDataDirty();
}

MeshData &UAnyMeshWidgetComponent::GetMeshDataRef(){
	FlagMeshDataDirty();
	return assignedMeshData;
}


bool UAnyMeshWidgetComponent::RayIntersect(
    const FVector &origin,
    const FVector &direction
){	
	/*
	FVector localOrigin = GetComponentTransform().InverseTransformPosition(origin);
	FVector localDirection = GetComponentTransform().InverseTransformVector(direction);

	DebugHelper::logMessage("UAnyMeshWidgetComponent::RayIntersect Triggered!");

	FIntersectHitResult outIntersectHitResult;

	if(assignedMeshData.RayIntersectDraw(
		localOrigin,
		localDirection,
		outIntersectHitResult,
		GetWorld(),
		GetComponentTransform().ToMatrixWithScale()
	)){
		DebugHelper::logMessage(
			FString::Printf(
				TEXT("UAnyMeshWidgetComponent::RayIntersect Mesh Hit!, %.2f %.2f"),
				outIntersectHitResult.hitUV.X,
				outIntersectHitResult.hitUV.Y
			)
		);

		//create screen coordinate from UV
		FVector2D fixedUv = outIntersectHitResult.hitUV; //= ToScreenUV(outIntersectHitResult.hitUV);
		FVector2D screen = WidgetScreenPosition(fixedUv);
		// dispatch click to widget
		if(UWidget *w = GetWidget()){
			//cast or click dispatcher
			if(IBaseUiInterface *casted = Cast<IBaseUiInterface>(w)){
				casted->dispatchClick(screen);
			}
		}
		return true;
	}
	return false;*/

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

// ---- shared mesh data hittest ----
FAnyMeshWidgetRayIntersectResult UAnyMeshWidgetComponent::RayIntersectResult(
	const FVector &origin, 
	const FVector &direction
){
	//saves the result in shared struct
	FAnyMeshWidgetRayIntersectResult widgetDispatchResult;

	FVector localOrigin = GetComponentTransform().InverseTransformPosition(origin);
	FVector localDirection = GetComponentTransform().InverseTransformVector(direction);

	FIntersectHitResult meshDataOutIntersectHitResult;
	if(assignedMeshData.RayIntersect(//RayIntersectDraw(
		localOrigin,
		localDirection,
		meshDataOutIntersectHitResult
		//,
		//GetWorld(),
		//GetComponentTransform().ToMatrixWithScale()
	)){
		DebugHelper::logMessage(
			FString::Printf(
				TEXT("UAnyMeshWidgetComponent::RayIntersect Mesh Hit!, %.2f %.2f"),
				meshDataOutIntersectHitResult.hitUV.X,
				meshDataOutIntersectHitResult.hitUV.Y
			)
		);

		//create screen coordinate from UV
		FVector2D fixedUv = meshDataOutIntersectHitResult.hitUV; //= ToScreenUV(outIntersectHitResult.hitUV);
		FVector2D screen = WidgetScreenPosition(fixedUv);

		widgetDispatchResult.SetResult(screen);
	}

	return widgetDispatchResult;
}

//if the triangle buffer is set up as 0,1,2 and the uvs with 0,0 at 0,
//flip the uv to screen coordonate as followed:
FVector2D UAnyMeshWidgetComponent::ToScreenUV(const FVector2D &other){
	/*
	//flip uv vertical axis
		/ *
		meshdata
		1  2
		0  3

		screen uv
		0  1
		2  3
		* /
		uv.Y = 1.0 - uv.Y;

	*/
	FVector2D copy = other;
	copy.Y = 1.0f - other.Y;
	return copy;
}

FVector2D UAnyMeshWidgetComponent::WidgetScreenPosition(
    FVector2D uv
){
	
	//calculate screen pos
	FVector2D WidgetSize = GetDrawSize();
	FVector2D Pivot = GetPivot();

	FVector2D Pixel(
		uv.X * WidgetSize.X,
		uv.Y * WidgetSize.Y
	);
	//Pixel += Pivot;//in screen or uv space? Debug needed!

	Pixel.X = FMath::Clamp(Pixel.X, 0.0f, WidgetSize.X);
	Pixel.Y = FMath::Clamp(Pixel.Y, 0.0f, WidgetSize.Y);
	return Pixel;
}

//Widget->SetDrawSize(size);
//Widget->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f)); 



/// ------------  CUSTOM PROXY ------------

FPrimitiveSceneProxy* UAnyMeshWidgetComponent::CreateSceneProxy()
{
	if (Space == EWidgetSpace::Screen) //never true.
	{
		return nullptr;
	}

    ///CurrentSlateWidget is private inside the
    //widget component
    //but we can say that only UUserWidgets are allowed
    //with:
	//DebugHelper::showScreenMessage("UAnyMeshWidgetComponent::CreateProxy Called");

	CreateMaterial();

	//if (WidgetRenderer && CurrentSlateWidget.IsValid()) //private member var. 
    if(WidgetRenderer && GetWidget() && GetMaterialInstance())
	{
		//is called every frame.
		//DebugHelper::showScreenMessage("UAnyMeshWidgetComponent::CreateProxy Valid Called");

		RequestRenderUpdate();
		LastWidgetRenderTime = 0;

		return new FDynamicMeshWidgetSceneProxy(
			this, 
			*WidgetRenderer->GetSlateRenderer(),
			assignedMeshData //copy overhead (?)
		);
	}
    return Super::CreateSceneProxy();
}

void UAnyMeshWidgetComponent::CreateMaterial(){
	if(!GetMaterialInstance()){

		UMaterialInterface *BaseMat = nullptr;
		if(assetManager *a = assetManager::instance()){
			DebugHelper::logMessage("UAnyMeshWidgetComponent::TryFindMaterial");
			BaseMat = a->Find<materialEnum, UMaterial>(materialEnum::widgetMaterial);
		}
		if(!BaseMat){
			BaseMat = UMaterial::GetDefaultMaterial(MD_Surface);
		}
		if (BaseMat){
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
		}
	}
}


UBodySetup* UAnyMeshWidgetComponent::GetBodySetup(){
	//custom override:
	UpdateBodySetupOverride();
	return BodySetup;
	// return Super::GetBodySetup();
}


void UAnyMeshWidgetComponent::UpdateBodySetupOverride()
{
	//MeshData::FVector center();


	if (Space == EWidgetSpace::Screen)
	{
		// We do not have a body setup in screen space
		BodySetup = nullptr;
	}
	else if ( !BodySetup || MeshDataWasModified)//bDrawSizeChanged )
	{
		BodySetup = NewObject<UBodySetup>(this);
		BodySetup->CollisionTraceFlag = CTF_UseSimpleAsComplex;
		BodySetup->AggGeom.BoxElems.Add(FKBoxElem());

		
		FKBoxElem* BoxElem = BodySetup->AggGeom.BoxElems.GetData();

		assignedMeshData.Update(BoxElem);

		//prints as expected and collision looks good.
		//DebugHelper::logMessage("UAnyMeshWidgetComponent::UpdateBodySetupOverride");

		/*
		const float Width = ComputeComponentWidth();
		const float Height = CurrentDrawSize.Y;
		const FVector Origin = FVector(.5f,
			-( Width * 0.5f ) + ( Width * Pivot.X ),
			-( Height * 0.5f ) + ( Height * Pivot.Y ));
			
		BoxElem->X = 0.01f;
		BoxElem->Y = Width;
		BoxElem->Z = Height;

		BoxElem->SetTransform(FTransform::Identity);
		BoxElem->Center = Origin;
		*/
	}
}


FCollisionShape UAnyMeshWidgetComponent::GetCollisionShape(float Inflation) const {
	FVector Extent = assignedMeshData.extent() + FVector(Inflation);
	DebugHelper::logMessage("UAnyMeshWidgetComponent::GetCollisionShape");
	return FCollisionShape::MakeBox(Extent);
	
	/*const FVector Extent = GetLocalBounds().BoxExtent + FVector(Inflation);
    return FCollisionShape::MakeBox(Extent);*/
}





void UAnyMeshWidgetComponent::UpdateInteractKeyHoldState(){
	isInteractHoldDown = UInteractionComponentHoveredCache::IsInteractKeyHoldWidgetComponent(this);
	if(IBaseUiInterface *casted = GetWidgetAsIBaseUiInterface()){
		casted->UpdateInteractKeyHoldState(isInteractHoldDown);
	}
}