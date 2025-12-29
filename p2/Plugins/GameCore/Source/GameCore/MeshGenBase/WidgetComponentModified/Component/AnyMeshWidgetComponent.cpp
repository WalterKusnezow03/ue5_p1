#include "AnyMeshWidgetComponent.h"

#include "DynamicMeshWidgetSceneProxy.h"
/*
UAnyMeshWidgetComponent::UAnyMeshWidgetComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {

      };*/


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

/// ------------  TO BE CHANGED ------------

FPrimitiveSceneProxy* UAnyMeshWidgetComponent::CreateSceneProxy()
{
	if (Space == EWidgetSpace::Screen)
	{
		return nullptr;
	}

    ///CurrentSlateWidget is private inside the
    //widget component
    //but we can say that only UUserWidgets are allowed
    //with:
	DebugHelper::showScreenMessage("UAnyMeshWidgetComponent::CreateProxy Called");

	CreateMaterial();

	//if (WidgetRenderer && CurrentSlateWidget.IsValid()) //private member var. 
    if(WidgetRenderer && GetWidget() && GetMaterialInstance())
	{
		DebugHelper::showScreenMessage("UAnyMeshWidgetComponent::CreateProxy Valid Called");

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

		// Default Material nehmen
		UMaterialInterface* BaseMat = UMaterial::GetDefaultMaterial(MD_Surface); // oder UMaterial::GetDefaultMaterial(MD_Surface)
		if (BaseMat)
		{
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
		}
	}
}