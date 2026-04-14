#include "NoMeshWidgetComponent.h"
#include "DebugPlugin/DebugHelper.h"


void UNoMeshWidgetComponent::BeginPlay(){
	Super::BeginPlay();

	//hide widget but still render
	

	materialUpdated = false;

	decal = FindDecal();
	SetResolutionFromComponent();
	RegisterWidgetToEventDispatcherOnBeginPlay();
	

	
	

	//WidgetComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel
	SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); // dein Channel


}




UDecalComponent *UNoMeshWidgetComponent::FindDecal(){      
    TArray<USceneComponent *> childs = GetAttachChildren();
	for (int i = 0; i < childs.Num(); i++){
		if(USceneComponent *current = childs[i]){
			if(UDecalComponent *casted = Cast<UDecalComponent>(current)){
				DebugHelper::logMessage("UNoMeshWidgetComponent::found decal");
				return casted;
			}
		}
	}
    return nullptr;
}




void UNoMeshWidgetComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateWidgetMaterialCustom();
	UpdateDecalMaterial();
}

//set material which is invisible in viewport.
void UNoMeshWidgetComponent::UpdateWidgetMaterialCustom(){
	if(!materialWidgetInitied && widgetMaterialInvisible){
		SetMaterial(0, widgetMaterialInvisible.Get());
		materialWidgetInitied = true;
		DebugHelper::logMessage("UNoMeshWidgetComponent::loadedCustomMaterial");
	}
}








void UNoMeshWidgetComponent::UpdateDecalMaterial()
{
	if(materialUpdated)
		return;

	if(!decal){
		//DebugHelper::logMessage("UNoMeshWidgetComponent::no decal");
		return;
	}

	if(!decalBaseMaterialRef.Get()){
		DebugHelper::logMessage("UNoMeshWidgetComponent::no decal material");
		return;
	}

	if(!GetWidget())
		InitWidget();

	UTextureRenderTarget2D* RT = GetRenderTarget();

	if (!RT){
		DebugHelper::showScreenMessage("UNoMeshWidgetComponent::NO RENDERTARGET");
		return;
	}
		

	UMaterialInstanceDynamic* DecalMat =
		UMaterialInstanceDynamic::Create(decalBaseMaterialRef.Get(), this);

	DecalMat->SetTextureParameterValue("SlateUI", RT);

	decal->SetDecalMaterial(DecalMat);
	decal->MarkRenderStateDirty();

	DebugHelper::logMessage("UNoMeshWidgetComponent::update parent material");

	materialUpdated = true;
}









void UNoMeshWidgetComponent::SetResolutionFromComponent(){

	if(!decal)
		return;

	FVector Size = decal->DecalSize;
	FVector2D PixelSize(Size.Y, Size.Z);

	
	//SetResolution(PixelSize);

	FVector TransformScale = decal->GetRelativeScale3D();
	//TransformScale *= 50.0f;
	FVector2D TransformScale2D(TransformScale.Y, TransformScale.Z);

	//ok aber falsch
	//PixelSize.X *= TransformScale.X;
	//PixelSize.Y *= TransformScale.Y;

	//besser aber leicht stretch
	PixelSize.X *= TransformScale.Y;
	PixelSize.Y *= TransformScale.X;

	SetResolution(PixelSize);

	DebugHelper::logMessage(FString::Printf(
		TEXT("UNoMeshWidgetComponent::DecalSize: %.2f %.2f, TransformScale %.2f %.2f %.2f"),
		PixelSize.X,
		PixelSize.Y,
		TransformScale.X,
		TransformScale.Y,
		TransformScale.Z
	));

}

void UNoMeshWidgetComponent::SetResolution(FVector2D res){
	//has to be flipped. Is correct like this for my use case.
	
	SetDrawSize(FIntPoint(res.Y, res.X)); //wrong (?)

	//SetDrawSize(FIntPoint(res.X, res.Y)); //correct (?)
}





void UNoMeshWidgetComponent::RegisterWidgetToEventDispatcherOnBeginPlay(){
    if(AEventDispatcherBase *instance = AEventDispatcherBase::MakeInstance(GetWorld())){
		WidgetIdKey key = MakeKey();
		AEventDispatcherBase::StaticRegister(key, TGetWidget<UEventWidgetBase>());
		DebugHelper::logMessage("UNoMeshWidgetComponent::static register");
	}
}

void UNoMeshWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
    UnRegisterWidgetToEventDispatcherOnEndPlay();
    Super::EndPlay(EndPlayReason);
}

void UNoMeshWidgetComponent::UnRegisterWidgetToEventDispatcherOnEndPlay(){
    WidgetIdKey key = MakeKey();
    AEventDispatcherBase::StaticUnRegister(key);
}

WidgetIdKey UNoMeshWidgetComponent::MakeKey(){
    WidgetIdKey key;
    key.Setup(actorName, widgetName);
    return key;
}
