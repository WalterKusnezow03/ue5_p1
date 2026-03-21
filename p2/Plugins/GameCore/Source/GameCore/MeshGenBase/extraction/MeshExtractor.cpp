// Fill out your copyright notice in the Description page of Project Settings.
#include "MeshExtractor.h"


#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h" 
#include "GameFramework/Actor.h"

#include "Engine/StaticMeshActor.h"
#include "MeshDescription.h"

#include "DebugPlugin/DebugHelper.h"

#include "Kismet/GameplayStatics.h"


//collect adges for an single actor passed per pointer
//data will get append to array
void MeshExtractor::ExtractMeshDataPerSection(AActorBase* actor, TArray<MeshData> &data){
    if(actor){
        TArray<UStaticMeshComponent *> componentsFound;
        actor->TFindAllChildsByType<UStaticMeshComponent>(componentsFound);

        MeshExtractorBase::ExtractMeshDataPerSection(componentsFound, data);
    }
}

//single meshdata


void MeshExtractor::ExtractSingleMeshData(AActorBase* actor, MeshData &meshdata){
    if(actor){
        TArray<UStaticMeshComponent *> componentsFound;
        actor->TFindAllChildsByType<UStaticMeshComponent>(componentsFound);
        MeshExtractorBase::ExtractSingleMeshData(componentsFound, meshdata);
    }
}
