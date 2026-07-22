#include "ReloadToolKit.h"




//call with: actor: this->GetRootComponent()

void ReloadToolKit::Setup(AActor *weapon, UStaticMeshComponent *mag){
    if(weapon && mag){
        USceneComponent *root = weapon->GetRootComponent();
        Setup(root, mag);
    }
}



void ReloadToolKit::Setup(USceneComponent *root, UStaticMeshComponent *mag){
    if(root && mag){
        mag1 = mag;
        mag2 = Duplicate(root, mag);

        magOriginalRelativeLocation = mag->GetComponentLocation();
        magOriginalRelativeRotation = mag->GetComponentRotation();
    }
}

UStaticMeshComponent *ReloadToolKit::Duplicate(USceneComponent *root, UStaticMeshComponent *OriginalMeshComp){
    if (root && OriginalMeshComp)
    {
        UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(root, UStaticMeshComponent::StaticClass());
        if (NewMeshComp){
            NewMeshComp->SetStaticMesh(OriginalMeshComp->GetStaticMesh());
            NewMeshComp->SetMobility(OriginalMeshComp->Mobility);
            USceneComponent* OriginalParent = OriginalMeshComp->GetAttachParent();
            if (OriginalParent){
                FName TargetSocket = OriginalMeshComp->GetAttachSocketName();
                NewMeshComp->AttachToComponent(OriginalParent, FAttachmentTransformRules::KeepRelativeTransform, TargetSocket);
            }else{
                NewMeshComp->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
            }
            NewMeshComp->SetRelativeTransform(OriginalMeshComp->GetRelativeTransform());
            NewMeshComp->RegisterComponent();
            return NewMeshComp;
        }
    }
    return nullptr;
}


//perform empty reload: mag wegwerfen


//perform reload swap mit parelleler animation


//für die animationen Bspline interp?



void ReloadToolKit::TickReload(float DeltaTime){
    
}


/*
//mag 2 annähern
KeyFrameAnimation magMoveToWeapon;

//mag wechseln
KeyFrameAnimation paralellReload;

//mag 1 weg tun
KeyFrameAnimation magPutAway;

//mag 1 hide

//swap mag ptrs
*/
