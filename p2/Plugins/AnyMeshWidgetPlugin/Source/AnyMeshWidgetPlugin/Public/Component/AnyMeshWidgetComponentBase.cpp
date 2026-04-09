#include "AnyMeshWidgetComponentBase.h"

#include "DynamicMeshWidgetSceneProxy.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/intersectCache/IntersectHitResult.h"


void UAnyMeshWidgetComponentBase::BeginPlay(){
	Super::BeginPlay();
	//CreateMaterialFromPath();

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

	//must happen here
	CreateMaterialFromPath();

	if (MeshDataWasModified)
	{
		assignedMeshData.RebuildBounds(); //unklar ob hier gebraucht
		MarkRenderStateDirty();		 // CreateSceneProxy() will be called again.
		MeshDataWasModified = false; //Reset flag since new proxy is created!


		//recreated collision box since 
		//mesh data was modified! (calls UBodySetup Override again!)
		MeshDataWasModifiedRecreatePhysicsState = true;
		RecreatePhysicsState();
	}
	
}

void UAnyMeshWidgetComponentBase::FlagMeshDataDirty(){
	MeshDataWasModified = true;
}

void UAnyMeshWidgetComponentBase::OverrideMeshData(MeshData &dataIn){
	assignedMeshData = dataIn;
	FlagMeshDataDirty();
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
	if(!DrawRayIntersectTest){
		if(assignedMeshData.RayIntersect(
			localOrigin,
			localDirection,
			meshDataOutIntersectHitResult
		)){
			//create screen coordinate from UV
			FVector2D fixedUv = meshDataOutIntersectHitResult.hitUV; //= ToScreenUV(outIntersectHitResult.hitUV);
			FVector2D screen = WidgetScreenPosition(fixedUv);

			widgetDispatchResult.SetResult(screen);
		}
	}else{
		if(assignedMeshData.RayIntersectDraw(
			localOrigin,
			localDirection,
			meshDataOutIntersectHitResult,
			GetWorld(),
			GetComponentTransform().ToMatrixWithScale()
		)){
			//create screen coordinate from UV
			FVector2D fixedUv = meshDataOutIntersectHitResult.hitUV; //= ToScreenUV(outIntersectHitResult.hitUV);
			FVector2D screen = WidgetScreenPosition(fixedUv);

			widgetDispatchResult.SetResult(screen);
		}
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
			assignedMeshData, // copy overhead (?)
			allowRender
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
	if(MeshDataWasModifiedRecreatePhysicsState){
		MeshDataWasModifiedRecreatePhysicsState = false;
		//DebugHelper::logMessage("UAnyMeshWidgetComponentBase::GetBodySetup After recreate physics");
	}

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
		return;
	}

	//only create if still needed
	if(!BodySetup){
		BodySetup = NewObject<UBodySetup>(this);
		BodySetup->CollisionTraceFlag = CTF_UseSimpleAsComplex;
		//BodySetup->AggGeom.BoxElems.Add(FKBoxElem());
		BodySetup->AggGeom.BoxElems.SetNum(1);
	}

	//update from mesh data
	FKBoxElem* BoxElem = BodySetup->AggGeom.BoxElems.GetData();
	assignedMeshData.Update(BoxElem);
	
}


FCollisionShape UAnyMeshWidgetComponentBase::GetCollisionShape(float Inflation) const {
	FVector Extent = assignedMeshData.extent() + FVector(Inflation);
	//DebugHelper::logMessage("UAnyMeshWidgetComponent::GetCollisionShape");
	return FCollisionShape::MakeBox(Extent);
	
	/*const FVector Extent = GetLocalBounds().BoxExtent + FVector(Inflation);
    return FCollisionShape::MakeBox(Extent);*/
}



#include "DebugPlugin/DebugHelper.h"
void UAnyMeshWidgetComponentBase::SetResolution(FVector2D res){
	//has to be flipped. Is correct like this for my use case.
	SetDrawSize(FIntPoint(res.Y, res.X));


	/*//if render target is wanted
	if(WidgetRenderTarget){
		// Alte GPU-Ressourcen freigeben und neue Größe setzen

		WidgetRenderTarget->InitCustomFormat(FMath::RoundToInt(res.X), FMath::RoundToInt(res.Y), PF_FloatRGBA, false);
	}*/
        

    

}














//// ---- custom material , lit materials ----

void UAnyMeshWidgetComponentBase::CreateMaterialFromPath(){
	if(bNeedsUpdateMaterial){
		//return; //DEBUG
		if (MaterialPath.Len() <= 0)
		{
			return;
		}
		if(UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr, *MaterialPath)){

			//ACHTUNG!:
			//material instance wird
			//intern erstellt und slate params zugewiesen!
			//nicht hier machen!!
			SetMaterial(0, BaseMat);

			bNeedsUpdateMaterial = false;
			DebugHelper::logMessage("UAnyMeshWidgetComponentBase::loadedCustomMaterial");
		}
	}


	
}

