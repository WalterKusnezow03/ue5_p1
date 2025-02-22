// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "customMeshActorWater.h"


void AcustomMeshActorWater::BeginPlay(){
    Super::BeginPlay();
}

void AcustomMeshActorWater::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    MeshData *data = nullptr;
    if (meshLayersMap.find(LAYER) != meshLayersMap.end()){
        //find meshData from map by reference
        data = &meshLayersMap[LAYER];
    }
    TickWaterFunction(data, DeltaTime);

    refreshMesh();
}

void AcustomMeshActorWater::createPane(int sizeX, int sizeY, int detail){

    MeshData outputData;

    for (int x = 0; x < sizeX; x += detail){
        for (int y = 0; y < sizeX; y += detail){
            /*
            1 2
            0 3
            */
            FVector a(x, y, 0);
            FVector b(x, y + detail, 0);
            FVector c(x + detail , y + detail, 0);
            FVector d(x + detail , y, 0);

            Super::createQuad(a, b, c, d, outputData);
        }
    }

    outputData.calculateNormals();

    bool createNormals = false;
    Super::updateMesh(outputData, createNormals, LAYER);
}


void AcustomMeshActorWater::TickWaterFunction(MeshData *data, float DeltaTime){
    if(data != nullptr){

        // Beispiel: Definiere Parameter für die Wellenbewegung
        float Amplitude = 50.0f; // Maximale Wellenhöhe
        float Frequency = 1.0f;  // Frequenz der Welle (d.h. wie schnell sie schwingt)
        float Speed = 2.0f;      // Geschwindigkeit der Wellenbewegung

        TArray<FVector> &vertecies = data->getVerteciesRef();
        for (int i = 0; i < vertecies.Num(); i++){
            FVector &vertex = vertecies[i];
            float TimeFactor = (i + 1) * Frequency + Speed * DeltaTime;
            vertex.Z += Amplitude * FMath::Sin(TimeFactor);
        }

    }
}





void AcustomMeshActorWater::refreshMesh(){
    MeshData *data = nullptr;
    if (meshLayersMap.find(LAYER) != meshLayersMap.end()){
        //find meshData from map by reference
        data = &meshLayersMap[LAYER]; //hier mit eckigen klammern weil .find ein iterator ist
    }

    bool createNormals = false;
    if (data != nullptr && Mesh != nullptr)
    {

        if(createNormals){
            data->calculateNormals();
        }

        /**
         * example: 
         * 
        Mesh->CreateMeshSection(
            layer, 
            newvertecies, 
            this->triangles, 
            normals, 
            UV0, 
            VertexColors, 
            Tangents, 
            true
        );*/
        Mesh->ClearMeshSection(LAYER);
        Mesh->CreateMeshSection(
            LAYER, 
            data->getVerteciesRef(),//newvertecies, 
            data->getTrianglesRef(),//this->triangles, 
            data->getNormalsRef(),//normals, 
            data->getUV0Ref(),//UV0, 
            data->getVertexColorsRef(),//VertexColors, 
            data->getTangentsRef(),//Tangents, 
            true
        );

        //set for spehere overlap
        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
        Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    }
}