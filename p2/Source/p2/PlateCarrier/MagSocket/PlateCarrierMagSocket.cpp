#include "PlateCarrierMagSocket.h"


bool UPlateCarrierMagSocket::HasMag(){
    return insertedMag != nullptr;
}

bool UPlateCarrierMagSocket::IsType(EMagSocketType typeIn){
    return socketType == typeIn;
}

USceneComponent *UPlateCarrierMagSocket::GetPouchedMag(){
    return insertedMag;
}


void UPlateCarrierMagSocket::InsertMagSnap(USceneComponent *mag){
    if(mag != nullptr){
        insertedMag = nullptr; //must be removed earlier in some way - happens inside swap process

        if(mag != nullptr){
            insertedMag = mag;
            insertedMag->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
            RemoveOffset(insertedMag);
        }
    }
}

void UPlateCarrierMagSocket::DestroyMag(){
    if(insertedMag){
        USceneComponent *ptr = insertedMag;
        insertedMag = nullptr;
        ptr->DestroyComponent(); // detach auto (?)
        
    }
}


void UPlateCarrierMagSocket::ReplaceMag(USceneComponent *magIn){
    if(magIn){
        if(USceneComponent *generated = Duplicate(magIn)){
            DestroyMag();
            InsertMagSnap(generated);
            DisableCollision();
        }
    }
}

USceneComponent *UPlateCarrierMagSocket::Duplicate(USceneComponent *OriginalMeshComp){
    return Duplicate(this, OriginalMeshComp);
}

USceneComponent *UPlateCarrierMagSocket::Duplicate(USceneComponent *root, USceneComponent *OriginalMeshComp){
    if(root){
        if(UStaticMeshComponent *castedStaticMesh = Cast<UStaticMeshComponent>(OriginalMeshComp)){
            return DuplicateStaticMesh(root, castedStaticMesh);
        }
        if(USkeletalMeshComponent *castedSkeletal = Cast<USkeletalMeshComponent>(OriginalMeshComp)){
            return DuplicateSkeletal(root, castedSkeletal);
        }
    }
    return nullptr;
}


USceneComponent *UPlateCarrierMagSocket::DuplicateStaticMesh(USceneComponent *root, UStaticMeshComponent *OriginalMeshComp){
    if (root && OriginalMeshComp)
    {
        // Generiert einen garantiert einzigartigen Namen basierend auf dem Original
        FName UniqueName = MakeUniqueObjectName(root, UStaticMeshComponent::StaticClass(), *FString::Printf(TEXT("%s_Cloned"), *OriginalMeshComp->GetName()));
        UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(root, UStaticMeshComponent::StaticClass(), UniqueName);
        
        if (NewMeshComp){
            NewMeshComp->SetStaticMesh(OriginalMeshComp->GetStaticMesh());
            NewMeshComp->SetMobility(OriginalMeshComp->Mobility);
            
            // Materialien kopieren (wichtig für Material-Overrides auf dem Magazin!)
            int32 NumMaterials = OriginalMeshComp->GetNumMaterials();
            for (int32 i = 0; i < NumMaterials; ++i)
            {
                NewMeshComp->SetMaterial(i, OriginalMeshComp->GetMaterial(i));
            }

            USceneComponent* OriginalParent = OriginalMeshComp->GetAttachParent();
            if (OriginalParent){
                FName TargetSocket = OriginalMeshComp->GetAttachSocketName();
                // Zwingt die Komponente, ihre absolute Weltposition/-skalierung beim Anheften zu behalten
                NewMeshComp->AttachToComponent(OriginalParent, FAttachmentTransformRules::KeepWorldTransform, TargetSocket);
            }else{
                NewMeshComp->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
            }
            
            // Holt die exakte Transformation aus der Welt und brennt sie auf die neue Komponente auf
            NewMeshComp->SetWorldTransform(OriginalMeshComp->GetComponentTransform());
            NewMeshComp->RegisterComponent();
            
            return NewMeshComp;
        }
    }
    return nullptr;
}

USceneComponent* UPlateCarrierMagSocket::DuplicateSkeletal(
    USceneComponent* Root, 
    USkeletalMeshComponent* OriginalMeshComp
){
    if (Root && OriginalMeshComp)
    {
        USkeletalMeshComponent* NewMeshComp = NewObject<USkeletalMeshComponent>(Root, USkeletalMeshComponent::StaticClass());
        if (NewMeshComp)
        {
            NewMeshComp->SetSkeletalMeshAsset(OriginalMeshComp->GetSkeletalMeshAsset());
            NewMeshComp->SetMobility(OriginalMeshComp->Mobility);

            USceneComponent* OriginalParent = OriginalMeshComp->GetAttachParent();
            if (OriginalParent)
            {
                FName TargetSocket = OriginalMeshComp->GetAttachSocketName();
                // Zwingt die Komponente, ihre absolute Weltposition/-skalierung beim Anheften zu behalten
                NewMeshComp->AttachToComponent(OriginalParent, FAttachmentTransformRules::KeepWorldTransform, TargetSocket);
            }
            else
            {
                NewMeshComp->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);
            }

            // Holt die exakte Transformation aus der Welt und brennt sie auf die neue Komponente auf
            NewMeshComp->SetWorldTransform(OriginalMeshComp->GetComponentTransform());

            // --- SKELETAL-SPEZIFISCHE SCHRITTE ---
            NewMeshComp->SetAnimInstanceClass(OriginalMeshComp->GetAnimClass());

            int32 NumMaterials = OriginalMeshComp->GetNumMaterials();
            for (int32 i = 0; i < NumMaterials; ++i)
            {
                NewMeshComp->SetMaterial(i, OriginalMeshComp->GetMaterial(i));
            }

            if (OriginalMeshComp->GetPhysicsAsset())
            {
                NewMeshComp->SetPhysicsAsset(OriginalMeshComp->GetPhysicsAsset());
            }

            NewMeshComp->RegisterComponent();

            return NewMeshComp;
        }
    }
    return nullptr;
}

#include "GameCore/util/ActorBase/ActorBase.h"

void UPlateCarrierMagSocket::DisableCollision(){
    if(insertedMag){
        AActorBase::EnableCollisionOn(insertedMag, false);
    }   
}






void UPlateCarrierMagSocket::GetBoundsCenter(USceneComponent *comp, FVector &center){
    if(comp){
        FBoxSphereBounds ComponentBounds = comp->Bounds;
        FVector WorldCenter = ComponentBounds.Origin;
        center = comp->GetComponentTransform().InverseTransformPosition(WorldCenter);
    }
}

void UPlateCarrierMagSocket::RemoveOffset(USceneComponent *comp){
    if(comp){
        FVector center;
        GetBoundsCenter(comp, center);
        offsetRemoved = center;
        comp->SetRelativeLocation(-center);
    }
}



/*

FBoxSphereBounds ComponentBounds = magPointer->Bounds;

// 1. Get the Center (World Space Location)
FVector Center = ComponentBounds.Origin;

// 2. Get the Box Extent (Half-size: distance from center to edges)
FVector HalfSize = ComponentBounds.BoxExtent;

// 3. Get the Full Size (Width, Depth, Height of the bounding box)
FVector FullSize = HalfSize * 2.0f;

*/