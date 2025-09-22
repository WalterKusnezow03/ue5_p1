#include "MovingFoliageInstancerComponent.h"


void UMovingFoliageInstancerComponent::Init(int countChilds, UStaticMesh *someMesh, AActor *parent){

    if(!instancer && someMesh && parent){
        //UInstancedStaticMeshComponent isA UStaticMeshComponent
        instancer = NewObject<UInstancedStaticMeshComponent>(this);

        //UStaticMeshComponent::SetStaticMesh(UStaticMesh* NewMesh)
        instancer->SetStaticMesh(someMesh);
        instancer->RegisterComponent();
        //instancer->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
        //this->AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        
        //instancer = NewObject<UInstancedStaticMeshComponent>(parent);
        instancer->AttachToComponent(parent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);


        FTransform empty;
        for (int i = 0; i < countChilds; i++)
        {   
            //UInstancedStaticMeshComponent::AddInstanceById(FTransform, UStaticMesh)
            instancer->AddInstance(
                empty,
                bWorldSpaceFlag
            );
            //AddedChilds.Add(newChild);
            transformChilds.Add(empty);
        }
    }
}

void UMovingFoliageInstancerComponent::Update(const TArray<FVector> &positions){
    if(instancer){
        if(positions.Num() <= 0){
            SetHiddenInGame(true);
            return;
        }

        UpdateTransformArray(positions);
        int32 StartInstanceIndex = 0;
        bool bMarkRenderStateDirty = true;
        bool bTeleport = true;
        instancer->BatchUpdateInstancesTransforms(
            StartInstanceIndex,
            transformChilds,
            bWorldSpaceFlag,
            bMarkRenderStateDirty,
            bTeleport
        );
        SetHiddenInGame(false);
    }
    
    

    /*
    BatchUpdateInstancesTransforms (
        int32 StartInstanceIndex,
        const TArray< FTransform >& NewInstancesTransforms,
        bool bWorldSpace,
        bool bMarkRenderStateDirty,
        bool bTeleport
    )

    UpdateInstanceTransformById (
        FPrimitiveInstanceId InstanceId,
        const FTransform& NewInstanceTransform,
        bool bWorldSpace,
        bool bTeleport
    )
    */
}


void UMovingFoliageInstancerComponent::UpdateTransformArray(
    const TArray<FVector> &positions
){
    int limit = std::min(positions.Num(), transformChilds.Num());
    for (int i = 0; i < limit; i++)
    {
        FTransform &current = transformChilds[i];
        current.SetTranslation(positions[i]);
    }

    //make others dissappear
    if(limit < transformChilds.Num()){
        for (int i = limit; i < transformChilds.Num(); i++)
        {
            FVector pos(0, 0, -100000);
            FTransform &current = transformChilds[i];
            current.SetTranslation(pos);
        }
    }
}



/// @brief moves all instances away and hides this component
void UMovingFoliageInstancerComponent::HideAll(){
    SetHiddenInGame(true);
}

void UMovingFoliageInstancerComponent::SetHiddenInGame(bool flag){
    if(instancer){
        instancer->SetVisibility(flag, true); // Hide in game (and also affect children)
    }
}