#include "InteractWidgetActor.h"

#include "p2/ui/3Dui/GamePlayWidgets/InteractWidget/Widget/InteractWidget.h"
#include "DebugPlugin/DebugHelper.h"

#include "AssetPlugin/gamestart/assetManager.h"
#include "AssetEnumCollection/assetEnums/Eui3DWidgetEnum.h"

/*
AInteractWidgetActor *AInteractWidgetActor::MakeInstance(
    UWorld *world, 
    USceneComponent *attachTo,
    FVector relativeLocation
){
    //---> bricked!


    if(!attachTo){
        return nullptr;
    }
    if(!world){
        return nullptr;
    }

    UClass *SpawnClass = nullptr;

    if(assetManager *a = assetManager::instance()){
        SpawnClass = a->Find<Eui3DWidgetEnum, UClass>(Eui3DWidgetEnum::EInteractWidget);
    }
    if(!SpawnClass){
        DebugHelper::logMessage("AInteractWidgetActor:: widget BP not found");
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FVector Location(0, 0, 0);
    AInteractWidgetActor *spawned = world->SpawnActor<AInteractWidgetActor>(
        SpawnClass,
        Location,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (!spawned)
    {
        DebugHelper::logMessage("AInteractWidgetActor::MakeInstance - Spawn actor failed");
        return nullptr;
    }
    DebugHelper::logMessage("AInteractWidgetActor:: Spawn Success");


    //attach
    spawned->AttachToSceneWithRelativeLocation(attachTo, relativeLocation);

    return spawned;
}*/


void AInteractWidgetActor::InitFromObjectPool(){
    Super::InitFromObjectPool();

}

void AInteractWidgetActor::ReleaseToObjectPool(){
    Super::ReleaseToObjectPool();
    //reset flag?
}

void AInteractWidgetActor::BeginPlay(){
    Super::BeginPlay();
    InitInteractWidgetOnBeginPlay();
}

void AInteractWidgetActor::Tick(float deltatime){
    Super::Tick(deltatime);

    //ticked from component (!) - do not tick here.
    /*if(UInteractWidget *widget = GetInteractWidgetCasted()){
        widget->Tick(deltatime);
    }*/

}


void AInteractWidgetActor::InitInteractWidgetOnBeginPlay(){
    if(UInteractWidget *widget = GetInteractWidgetCasted()){
        widget->SetParentActor(this);
    }
}




void AInteractWidgetActor::Notify(){
    if(!isActive){
        return;
    }
    Super::NotifyAllCallbacks();
}

void AInteractWidgetActor::SetInteractWidgetActive(bool flag){
    isActive = flag;
}

UInteractWidget *AInteractWidgetActor::GetInteractWidgetCasted(){
    return GetWidget<UInteractWidget>();
}

//init on beginplay
void AInteractWidgetActor::CreateWidgetMeshData(){
    if(!bMeshDataCreated && Widget){
        float xMax = 300.0f;
        float yMax = 300.0f;

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

        //400cm
        ScaleMeshDataToMaxCm(50.0f);
    }
}
