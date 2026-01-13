#include "WeaponTableWidgetActor.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/WeaponTableActor.h"



void AWeaponTableWidgetActor::BeginPlay(){
    Super::BeginPlay();
    EnableCollision(true);
    debugUiActorName = TEXT("AWeaponTableWidgetActor");
}

AWeaponTableWidgetActor *AWeaponTableWidgetActor::MakeInstance(
    UWorld *world, 
    USceneComponent *attachTo,
    FVector relativeLocation
){
    if(!attachTo){
        return nullptr;
    }
    if(!world){
        return nullptr;
    }

    UClass *SpawnClass = nullptr;

    if(assetManager *a = assetManager::instance()){
        SpawnClass = a->Find<Eui3DWidgetEnum, UClass>(Eui3DWidgetEnum::EWeaponTableWidget);
    }
    if(!SpawnClass){
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FVector Location(0, 0, 0);
    AWeaponTableWidgetActor *spawned = world->SpawnActor<AWeaponTableWidgetActor>(
        SpawnClass,
        Location,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (!spawned)
    {
        DebugHelper::logMessage("AHudUiActor::MakeInstance - Spawn actor failed");
        return nullptr;
    }

    

    //attach
    spawned->AttachToScene(attachTo);
    spawned->SetActorRelativeLocation(relativeLocation);

    return spawned;
}


void AWeaponTableWidgetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
}

void AWeaponTableWidgetActor::AttachToScene(USceneComponent *attachTo){
    if(attachTo){
        AttachToComponent(attachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
}



UWeaponTableWidget *AWeaponTableWidgetActor::GetInnerWidget(){
    return GetWidget<UWeaponTableWidget>();
}

//init on beginplay
void AWeaponTableWidgetActor::CreateWidgetMeshData(){
    if(!bMeshDataCreated && Widget){
        float xMax = 1920.0f;
        float yMax = 1080.0f;

        /*
        1-->2
        |   |
        0<--3
        */

        FVector v0(0, 0, 0);
        FVector v1(0, 0, yMax);
        FVector v2(0, xMax, yMax);
        FVector v3(0, xMax, 0);

        MeshData &data = Widget->GetMeshDataRef();
        data.append(v0,v1,v2);
        data.append(v0,v2,v3);
        data.centerMesh();

        MMatrix scale;
        float factor = 0.1f;
        scale.scale(factor, factor, factor);
        data.transformAllVertecies(scale);

        FVector2D uv0(0, 1);
        FVector2D uv1(0, 0);
        FVector2D uv2(1, 0);
        FVector2D uv3(1, 1);
        data.appendUvs(uv0, uv1, uv2);
        data.appendUvs(uv0, uv2, uv3);

        data.calculateNormals();
        //data.flipNormals(); //shit happens
        

        bMeshDataCreated = true;

        SetDrawSize(FVector2D(xMax, yMax));
    }
}



// --- main functions ---

void AWeaponTableWidgetActor::SetWeaponSetupHelperRefernce(weaponSetupHelper *setup){
    if(setup){
        if(UWeaponTableWidget *widget = GetInnerWidget()){
            widget->SetWeaponSetupHelperRefernce(setup);
            widget->SetParentActor(this);
        }
    }
}


void AWeaponTableWidgetActor::NotifyWeaponSetupChange(){
    if(parentActor){
        parentActor->NotifyWeaponSetupChange();
    }
}

void AWeaponTableWidgetActor::SetParentActor(
    AWeaponTableActor *parent
){
    if(parent){
        parentActor = parent;
    }
}



void AWeaponTableWidgetActor::Tick(float deltatime){
    Super::Tick(deltatime);

}
