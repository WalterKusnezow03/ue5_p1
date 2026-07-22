#include "PlateCarrier.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/entityManager/EntityManager.h"
#include "AssetEnumCollection/assetEnums/EntityAsset.h"

APlateCarrier *APlateCarrier::MakeInstance(){
    if(EntityManager *ptr = EntityManager::instance()){
        return ptr->TESpawnActor<APlateCarrier, EntityAsset>(EntityAsset::PlateCarrier);
    }
    return nullptr;
}





APlateCarrier::APlateCarrier() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

void APlateCarrier::SetPlateCarrierOwner(IDamageinterface *interface){
    ownerDamageInterface = interface;
}

//methods
void APlateCarrier::takedamage(FCustomHitResult &result) {
    if(ownerDamageInterface != nullptr){
        ownerDamageInterface->takedamage(result);
    }
};

//actor location might not be enough for explosive / alert management
bool APlateCarrier::IsInRange(const FVector &position, float maxDistance){
    if(ownerDamageInterface){
        return ownerDamageInterface->IsInRange(position, maxDistance);
    }
    return FVector::Dist(position, GetActorLocation()) < maxDistance;
}

void APlateCarrier::setTeam(teamEnum t) {
    if(ownerDamageInterface){
        ownerDamageInterface->setTeam(t);
    }
    cachedTeam = t;
};

teamEnum APlateCarrier::getTeam() {
    if(ownerDamageInterface){
        return ownerDamageInterface->getTeam();
    }
    return teamEnum::none;
};



void APlateCarrier::BeginPlay(){
    Super::BeginPlay();
    DisableCollision();
    CollectSockets();
}

void APlateCarrier::DisableCollision(){
    EnableCollisionOnAllChilds(false);
}

void APlateCarrier::CollectSockets(){
    magSockets.Empty();
    FString namepart = "PlateCarrierMagSocket";
    TFindAllChildsByName<UPlateCarrierMagSocket>(namepart, magSockets);
}

void APlateCarrier::Tick(float deltatime){
    Super::Tick(deltatime);
    TickSwapProcess(deltatime);
}

void APlateCarrier::TickSwapProcess(float deltatime){
    
    if(swapProcess){
        if(swapProcess->IsFinished()){
            swapProcess = nullptr;
            return;
        }
        swapProcess->TickAnimation(deltatime);
    }
}





void APlateCarrier::FillMags(EMagSocketType typeSocket, USceneComponent *magPrefab){
    if(magPrefab){
        TArray<UPlateCarrierMagSocket*> found;
        FindAllMagSlots(typeSocket, found);
        for (int i = 0; i < found.Num(); i++){
            if(UPlateCarrierMagSocket *currentSocket = found[i]){
                currentSocket->ReplaceMag(magPrefab);
            }
        }
    }
}







bool APlateCarrier::StartSwapMag(EMagSocketType socketType, MagSwapProcess *process){
    
    if(process){
        if(UPlateCarrierMagSocket *swapSlot = FindMagFilledSlot(socketType)){
            swapProcess = process;
            DebugHelper::logMessage(
                "APlateCarrier<-Aweapon::TryReloadAnimationByPlateCarrier", 
                SocketTypeToString(socketType)
            );
            swapProcess->SetupAndStartAnimation(swapSlot);
            return true;
        }else{
            DebugHelper::logMessage("APlateCarrier<-Aweapon::TryReloadAnimationByPlateCarrier NO MAG FOUND");
            
        }
    }
    return false;
}

FString APlateCarrier::SocketTypeToString(EMagSocketType type){
    if(type == EMagSocketType::AR_MAG){
        return TEXT("_Mag_AR");
    }
    if(type == EMagSocketType::PISTOL_MAG){
        return TEXT("_Mag_Pistol");
    }
    return TEXT("_Mag_Other");
}

UPlateCarrierMagSocket *APlateCarrier::FindMagFilledSlot(){
    return FindMagFilledSlot(magSockets);
}
    

UPlateCarrierMagSocket *APlateCarrier::FindMagFilledSlot(EMagSocketType typeSocket){
    TArray<UPlateCarrierMagSocket*> found;
    FindAllMagSlots(typeSocket, found);
    return FindMagFilledSlot(found);
}

UPlateCarrierMagSocket *APlateCarrier::FindMagFilledSlot(TArray<UPlateCarrierMagSocket*> &inSockets){
    UPlateCarrierMagSocket *found = nullptr;
    for(int i = 0; i < inSockets.Num(); i++){
        if(UPlateCarrierMagSocket *current = inSockets[i]){
            if(current->HasMag()){
                found = current;
                return found;
            }
        }        
    }
    return found;
}

void APlateCarrier::FindAllMagSlots(
    EMagSocketType type, 
    TArray<UPlateCarrierMagSocket*> &outArray
){
    for(int i = 0; i < magSockets.Num(); i++){
        if(UPlateCarrierMagSocket *current = magSockets[i]){
            if(current->IsType(type)){
                outArray.Add(current);
            }
        }        
    }
}


void APlateCarrier::AttachToActor(AActor *other, const FVector &relativeLocationOverride){
    if(other){
        AttachToActor(other);
        if(USceneComponent *root = GetRootComponent()){
            root->SetRelativeLocation(relativeLocationOverride);
        }
    }
}

void APlateCarrier::AttachToActor(AActor *other){
    if(other){
        if(USceneComponent *otherRoot = other->GetRootComponent()){
            this->AttachToComponent(otherRoot, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
    


    /*// Ensure both actors exist before attaching
    if (other && TargetComponent)
    {
        // Configure how the actor behaves when attaching
        FAttachmentTransformRules AttachmentRules(
            EAttachmentRule::SnapToTarget, // Location: Match target/socket exactly
            EAttachmentRule::SnapToTarget, // Rotation: Match target/socket exactly
            EAttachmentRule::KeepWorld,    // Scale: Keep its own scale
            true                           // Weld simulated bodies (for physics)
        );

        // Attach the actor's root component to the target component
        MySpawnedActor->AttachToComponent(
            TargetComponent, 
            AttachmentRules, 
            TEXT("MySocketName") // Optional: Pass NAME_None if not using a socket/bone
        );
    }*/
}