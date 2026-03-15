#include "throwableWeaponBase.h"



void AthrowableWeaponBase::BeginPlay(){
    Super::BeginPlay();
    FindThrowableMeshOnBeginPlay();
    if(rootMesh){
        rootMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        rootMesh->SetCollisionResponseToAllChannels(ECR_Block);
        rootMesh->SetSimulatePhysics(false); // initially off
    }


    wasMarkedForThrow = false;
}

void AthrowableWeaponBase::FindThrowableMeshOnBeginPlay(){
    
    TTryAssignByNameContains<UPrimitiveComponent>(thrownMeshName, rootMesh);

    if(!rootMesh){
        DebugHelper::logMessage("AthrowableWeaponBase::rootStaticMesh NOT FOUND!");
    }
    
    SetPhysicsEnabled(false);
}




void AthrowableWeaponBase::MarkForApplyImpulse(FVector &direction){
    wasMarkedForThrow = true;
    throwDirection = direction;
    SetPhysicsEnabled(true);
}

void AthrowableWeaponBase::ApplyMarkedImpulse(){
    if(wasMarkedForThrow){
        DebugHelper::logMessage("AthrowableWeaponBase::ApplyMarkedImpulse A");
        if(rootMesh){
            UE_LOG(LogTemp, Warning, TEXT("AthrowableWeaponBase::CollisionEnabled=%d SimulatePhysics=%d"), 
                (int)rootMesh->GetCollisionEnabled(),
                (int)rootMesh->IsSimulatingPhysics());
        }
        

        if(throwDirection.Size() <= 0.0f){
            return;
        }

        if (rootMesh)
        {
            DebugHelper::logMessage("AthrowableWeaponBase::ApplyMarkedImpulse B");
            rootMesh->AddImpulse(throwDirection.GetSafeNormal() * throwableVelocity);
        }
    }
    wasMarkedForThrow = false;
}

void AthrowableWeaponBase::SetPhysicsEnabled(bool flag){
    if(rootMesh){
        if(flag)
        {
            rootMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            rootMesh->SetCollisionResponseToAllChannels(ECR_Block);
            rootMesh->SetSimulatePhysics(true);
            DebugHelper::logMessage("AthrowableWeaponBase::SetSimulatePhysics YES");
        }
        else
        {
            // Optional: QueryOnly, damit Unreal keine Fehler wirft
            rootMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            rootMesh->SetSimulatePhysics(false);
        }
    }
}

void AthrowableWeaponBase::ResetPhysics(){
    if (rootMesh)
    {
        rootMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        rootMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

        
        rootMesh->SetRelativeLocation(FVector::ZeroVector);
    }
}



FVector AthrowableWeaponBase::PhysicsMeshLocation(){
    if(rootMesh){
        return rootMesh->GetComponentLocation();
    }
    return GetActorLocation();
}

void AthrowableWeaponBase::Tick(float Deltatime){
    Super::Tick(Deltatime);
    ApplyMarkedImpulse();
    DrawLocation(Deltatime);

    //do not.
    //SetActorLocation(PhysicsMeshLocation());
}

/// --- keep empty for now ---

void AthrowableWeaponBase::shoot(){

}

void AthrowableWeaponBase::shootBot(FVector target){

}

void AthrowableWeaponBase::releaseShoot(){

}


// on pickup: disable physics!







void AthrowableWeaponBase::UpdateLocalSceneTransformCarriedByHand(
    EArmType type, 
    FVector &locationLocal, 
    FRotator &rotationLocal
){
    //exapmple implementation
    if(GetCarryType() == EIKCarryType::ECarryByHand){
        if(USceneComponent *component = FindHandCarriedScene(type)){
            component->SetRelativeLocation(locationLocal);
            component->SetRelativeRotation(rotationLocal);
        }
    }
}

USceneComponent *AthrowableWeaponBase::FindHandCarriedScene(EArmType type){
    return nullptr;
}





void AthrowableWeaponBase::DrawLocation(float deltatime){
    DebugHelper::showLineBetween(
        GetWorld(),
        GetActorLocation(),
        GetActorLocation() + FVector(0,0,100),
        FColor::Red,
        deltatime * 2.0f
    );
    
    
    if(rootMesh){
        DebugHelper::showLineBetween(
            GetWorld(),
            rootMesh->GetComponentLocation(),
            rootMesh->GetComponentLocation() + FVector(0,0,100),
            FColor::Green,
            deltatime * 2.0f
        );
        DebugHelper::showLineBetween(
            GetWorld(),
            rootMesh->GetComponentLocation(),
            GetActorLocation(),
            FColor::Orange,
            deltatime * 2.0f
        );
    }


}