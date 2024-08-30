// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "customMeshActor.h"

// Sets default values
AcustomMeshActor::AcustomMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the ProceduralMeshComponent
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    RootComponent = Mesh;

}

// Called when the game starts or when spawned
void AcustomMeshActor::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AcustomMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





/// @brief process a 2D map of local coordinates
/// @param map 2D vector of LOCAL coordinates!
void AcustomMeshActor::process2DMap(std::vector<std::vector<FVector>> &map){

    //of course it will copy thisis fine

    //die vertecies müssen dann so durchlaufen werden dass man sie immer nachbarn weise in dreiecken anordnet
    //am besten man iteriert jeweils bis 
    //map.size() - 1 damit man den offset ignorieren kann und es genau aufgeht.

    TArray<FVector> output;
    TArray<int32> newtriangles;
    TArray<FVector> normalsOutput;

    for (int x = 0; x < map.size() - 1; x++){
        for (int y = 0; y < map.at(x).size() - 1; y++){
            /*
                1--2
                |  |
                0<-3
             */
            bool copy = (x != 0); //prev 0 and 1 indices will be copied

            FVector vzero = map.at(x).at(y);
            FVector vone = map.at(x).at(y + 1);
            FVector vtwo = map.at(x + 1).at(y + 1);
            FVector vthree = map.at(x + 1).at(y);
            buildQuad(vzero, vone, vtwo, vthree, output, newtriangles);
        }
    }


    //process created data
    updateMesh(output, newtriangles);
}

void AcustomMeshActor::updateMesh(TArray<FVector> newvertecies, TArray<int32> newtriangles){
    triangles = newtriangles;
    vertecies = newvertecies;
    

    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> Tangents;
    TArray<FColor> VertexColors;

    // Create the mesh section
    
    Mesh->CreateMeshSection(0, this->vertecies, this->triangles, Normals, UV0, VertexColors, Tangents, true);

    FVector offset = GetActorLocation();
    offset.Z += 100;
    SetActorLocation(offset);
}

void AcustomMeshActor::buildQuad(
    FVector a, 
    FVector b, 
    FVector c, 
    FVector d, 
    TArray<FVector> &output,
    TArray<int32> &trianglesOutput
){

    buildTriangle(a, b, c, output, trianglesOutput);
    buildTriangle(a, c, d, output, trianglesOutput);
    return;

    //add vertecies
    output.Add(a);
    output.Add(b);
    output.Add(c);
    output.Add(d);

    //add triangles

    int32 offset = trianglesOutput.Num();

    trianglesOutput.Add(0 + offset); // 0th vertex in the first triangle
    trianglesOutput.Add(1 + offset); // 1st vertex in the first triangle
    trianglesOutput.Add(2 + offset); // 2nd vertex in the first triangle
    
    trianglesOutput.Add(0 + offset); // 0th vertex in the second triangle
    trianglesOutput.Add(2 + offset); // 2nd vertex in the second triangle
    trianglesOutput.Add(3 + offset); // 3rd vertex in the second triangle

    /*
                1--2
                |  |
                0<-3
    */

}




void AcustomMeshActor::buildTriangle(
    FVector a, 
    FVector b, 
    FVector c,
    TArray<FVector> &output,
    TArray<int32> &trianglesOutput
){
    //add vertecies
    output.Add(a);
    output.Add(b);
    output.Add(c);

    //add triangles

    int32 offset = trianglesOutput.Num();

    trianglesOutput.Add(0 + offset); // 0th vertex in the first triangle
    trianglesOutput.Add(1 + offset); // 1st vertex in the first triangle
    trianglesOutput.Add(2 + offset); // 2nd vertex in the first triangle
    
}


