#include "WeaponTableWidgetActor.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/WeaponTableActor.h"
#include "AssetEnumCollection/assetEnums/Eui3DWidgetEnum.h"


void AWeaponTableWidgetActor::BeginPlay(){
    Super::BeginPlay();
    EnableCollision(true);
    debugUiActorName = TEXT("AWeaponTableWidgetActor");

    if(UWeaponTableWidget *inner = GetInnerWidget()){
        inner->InitWidgets();
    }
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
        DebugHelper::logMessage("AWeaponTableWidgetActor::BP NOT FOUND");
        return nullptr;
    }
    DebugHelper::logMessage("AWeaponTableWidgetActor::BP FOUND");

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
        DebugHelper::logMessage("AWeaponTableWidgetActor::MakeInstance - Spawn actor failed");
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
        data.flipWindingOrder();

        bMeshDataCreated = true;

        SetDrawSize(FVector2D(xMax, yMax));

        ScaleMeshDataToDesiredScale();
    }
}

void AWeaponTableWidgetActor::ScaleMeshDataToDesiredScale(){
    ScaleMeshDataToMaxCm(scaleCmMeshDataTargeted);
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
    DebugHelper::logMessage("AWeaponTableWidgetActor:: Notified change");
    if (parentActor)
    {
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
