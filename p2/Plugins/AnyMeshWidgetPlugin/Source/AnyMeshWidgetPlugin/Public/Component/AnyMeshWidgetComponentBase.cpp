#include "AnyMeshWidgetComponentBase.h"

#include "DynamicMeshWidgetSceneProxy.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/intersectCache/IntersectHitResult.h"


void UAnyMeshWidgetComponentBase::BeginPlay(){
	Super::BeginPlay();

	//DebugHelper::logMessage("UAnyMeshWidgetComponent::BeginPlay");
	UMaterialInterface *BaseMat = nullptr;
	BaseMat = UMaterial::GetDefaultMaterial(MD_Surface);
	
	if (BaseMat){
		MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
	}

	//WidgetComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel
	SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel
}

void UAnyMeshWidgetComponentBase::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(MeshDataWasModified){
		MarkRenderStateDirty(); //CreateSceneProxy will be called again.
	}
}



void UAnyMeshWidgetComponentBase::FlagMeshDataDirty(){
	MeshDataWasModified = true;
}



MeshData &UAnyMeshWidgetComponentBase::GetMeshDataRef(){
	FlagMeshDataDirty();
	return assignedMeshData;
}


bool UAnyMeshWidgetComponentBase::RayIntersect(
    const FVector &origin,
    const FVector &direction
){	
	FAnyMeshWidgetRayIntersectResult result = RayIntersectResult(origin, direction);
	if(result.IsResultValid()){
		
		return true;
	}
	return false;
}

bool UAnyMeshWidgetComponentBase::RayIntersectHover(
	const FVector &origin,
	const FVector &direction
){
	FAnyMeshWidgetRayIntersectResult result = RayIntersectResult(origin, direction);
	if(result.IsResultValid()){
		return true;
	}
	return false;
}


// ---- shared mesh data hittest ----
FAnyMeshWidgetRayIntersectResult UAnyMeshWidgetComponentBase::RayIntersectResult(
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
		/*DebugHelper::logMessage(
			FString::Printf(
				TEXT("UAnyMeshWidgetComponent::RayIntersect Mesh Hit!, %.2f %.2f"),
				meshDataOutIntersectHitResult.hitUV.X,
				meshDataOutIntersectHitResult.hitUV.Y
			)
		);*/

		//create screen coordinate from UV
		FVector2D fixedUv = meshDataOutIntersectHitResult.hitUV; //= ToScreenUV(outIntersectHitResult.hitUV);
		FVector2D screen = WidgetScreenPosition(fixedUv);

		widgetDispatchResult.SetResult(screen);
	}

	return widgetDispatchResult;
}

//if the triangle buffer is set up as 0,1,2 and the uvs with 0,0 at 0,
//flip the uv to screen coordonate as followed:
FVector2D UAnyMeshWidgetComponentBase::ToScreenUV(const FVector2D &other){
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

FVector2D UAnyMeshWidgetComponentBase::WidgetScreenPosition(
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

FPrimitiveSceneProxy* UAnyMeshWidgetComponentBase::CreateSceneProxy()
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

void UAnyMeshWidgetComponentBase::CreateMaterial(){
	if(!GetMaterialInstance()){

		UMaterialInterface *BaseMat = nullptr;
		BaseMat = UMaterial::GetDefaultMaterial(MD_Surface);
		
		if (BaseMat){
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
		}
	}
}


UBodySetup* UAnyMeshWidgetComponentBase::GetBodySetup(){
	//custom override:
	UpdateBodySetupOverride();
	return BodySetup;
	// return Super::GetBodySetup();
}


void UAnyMeshWidgetComponentBase::UpdateBodySetupOverride()
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


FCollisionShape UAnyMeshWidgetComponentBase::GetCollisionShape(float Inflation) const {
	FVector Extent = assignedMeshData.extent() + FVector(Inflation);
	//DebugHelper::logMessage("UAnyMeshWidgetComponent::GetCollisionShape");
	return FCollisionShape::MakeBox(Extent);
	
	/*const FVector Extent = GetLocalBounds().BoxExtent + FVector(Inflation);
    return FCollisionShape::MakeBox(Extent);*/
}



