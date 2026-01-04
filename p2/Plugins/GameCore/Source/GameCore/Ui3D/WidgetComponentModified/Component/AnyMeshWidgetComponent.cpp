#include "AnyMeshWidgetComponent.h"

#include "DynamicMeshWidgetSceneProxy.h"
#include "GameCore/MeshGenBase/MeshData/intersectCache/IntersectHitResult.h"

#include "customuipluginbase/baseInterface/BaseUiInterface.h"
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
	FVector localOrigin = GetComponentTransform().InverseTransformPosition(origin);
	FVector localDirection = GetComponentTransform().InverseTransformPosition(direction);

	FIntersectHitResult outIntersectHitResult;
	if(assignedMeshData.RayIntersect(
		localOrigin,
		localDirection,
		outIntersectHitResult
	)){
		//create screen coordinate from UV
		FVector2D screen = WidgetScreenPosition(outIntersectHitResult.hitUV);
		// dispatch click to widget
		if(UWidget *w = GetWidget()){
			//cast or click dispatcher
			if(IBaseUiInterface *casted = Cast<IBaseUiInterface>(w)){
				casted->dispatchClick(screen);
			}
		}
		return true;
	}
	return false;
}

FVector2D UAnyMeshWidgetComponent::WidgetScreenPosition(
    const FVector2D &uv
){
	FVector2D WidgetSize = GetDrawSize();
	FVector2D Pivot = GetPivot();

	FVector2D Pixel(
		uv.X * WidgetSize.X,
		uv.Y * WidgetSize.Y
	);
	Pixel += Pivot;//in screen or uv space? Debug needed!

	Pixel.X = FMath::Clamp(Pixel.X, 0.f, WidgetSize.X);
	Pixel.Y = FMath::Clamp(Pixel.Y, 0.f, WidgetSize.Y);
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