// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/util/FVectorTouple.h"
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
/// correct position of the chunk must be set before!
/// @param map 2D vector of LOCAL coordinates!
void AcustomMeshActor::process2DMap(std::vector<std::vector<FVector>> &map){ //nach dem entity manager stirbt die refenz hier!

    //of course it will copy thisis fine

    //die vertecies müssen dann so durchlaufen werden dass man sie immer nachbarn weise in dreiecken anordnet
    //am besten man iteriert jeweils bis 
    //map.size() - 1 damit man den offset ignorieren kann und es genau aufgeht.

    TArray<FVector> output;
    TArray<int32> newtriangles;
    TArray<FVector> normalsOutput;

    TArray<FVectorTouple> touples; //first arg: center, second: normal

    for (int x = 0; x < map.size() - 1; x++){
        for (int y = 0; y < map.at(x).size() - 1; y++){
            /*
                1--2
                |  |
                0<-3
             */
            bool copy = (x != 0); //prev 0 and 1 indices will be copied


            if(x + 1 < map.size() && y + 1 < map.at(x + 1).size()){
                try{
                    FVector vzero = map.at(x).at(y);
                    FVector vone = map.at(x).at(y + 1);
                    FVector vtwo = map.at(x + 1).at(y + 1);
                    FVector vthree = map.at(x + 1).at(y);
                    buildQuad(vzero, vone, vtwo, vthree, output, newtriangles);

                    FVector normal = FVectorUtil::calculateNormal(vzero, vone, vtwo);
                    //calculate center
                    FVector center = FVectorUtil::calculateCenter(vzero, vone, vtwo);
                    center += GetActorLocation(); //correct offset

                    // create and add touple to list
                    FVectorTouple t(center, normal); // first center, then normal
                    touples.Add(t);
                }
                catch (const std::exception &e)
                {
                    DebugHelper::showScreenMessage("mesh actor exception!", FColor::Red);
                }
            }
            
        }
    }


    //process created data
    updateMesh(output, newtriangles);
    if(EntityManager *e = EntityManager::instance()){
        ApplyMaterial(Mesh, e->getMaterial(materialEnum::grassMaterial));
    }

    //iterate over touples and add foliage based on height and if the pane is flat or vertical
    createFoliage(touples);
}

/// @brief updates the mesh for the actor
/// @param newvertecies vertecies to set
/// @param newtriangles triangles for the vertecies
void AcustomMeshActor::updateMesh(TArray<FVector> newvertecies, TArray<int32> newtriangles){
    triangles = newtriangles;
    vertecies = newvertecies;
    

    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> Tangents;
    TArray<FColor> VertexColors;

    // Create the mesh section

    int sectionIndex = 0; //auch bei den materials, hier das erste argument, merken
    Mesh->CreateMeshSection(0, this->vertecies, this->triangles, Normals, UV0, VertexColors, Tangents, true);
    

    //debug offset
    //FVector offset = GetActorLocation();
    //offset.Z += 100;
    //SetActorLocation(offset);


    //new: set material
}

/// @brief build a quad out of two triangles! Important otherwise unfixable issues are in the mesh
/// @param a 
/// @param b 
/// @param c 
/// @param d 
/// @param output 
/// @param trianglesOutput 
void AcustomMeshActor::buildQuad(
    FVector a, 
    FVector b, 
    FVector c, 
    FVector d, 
    TArray<FVector> &output,
    TArray<int32> &trianglesOutput
){

    //must be individual triangles:
    //quads: buggy + the engine is converting it to triangles back again anyway
    buildTriangle(a, b, c, output, trianglesOutput);
    buildTriangle(a, c, d, output, trianglesOutput);
    return;

    /*
                1--2
                |  |
                0<-3
    */

}



/// @brief all quads MUST BE BUILD out of TRIANGLES, OTHERWISE MANY BUGS OCCUR!
/// @param a corner 0
/// @param b corner 1
/// @param c corner 2
/// @param output output to append in
/// @param trianglesOutput triangle int32 as nums saved in here, also appended
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


/// @brief applys a material to the whole component (slot 0)
/// @param ProceduralMeshComponent 
/// @param Material 
void AcustomMeshActor::ApplyMaterial(UProceduralMeshComponent* ProceduralMeshComponent, UMaterial* Material) {
	if (ProceduralMeshComponent && Material) {
		// Apply the material to the first material slot (index 0) of the procedural mesh
		ProceduralMeshComponent->SetMaterial(0, Material);
	}
}



/// @brief process touples with
/// @param touples vector of touples with each: (first: center of pane with correct offset, second: normal of pane)
void AcustomMeshActor::createFoliage(TArray<FVectorTouple> &touples){

    //iterate over touples
    //determine normal angle and apply foliage, rocks, trees accordingly

    //if normal faces towards up: flat area
    for(FVectorTouple &t : touples){

        FVector &location = t.first();
        FVector &normal = t.second();

        bool facingUpwards = FVectorUtil::directionIsVertical(normal);
        if(facingUpwards){
            //some debug drawing 
            DebugHelper::showLineBetween(GetWorld(), location, location + normal * 200, FColor::Purple);

            //pane is flat
            //spawn something at a location
            AActor *someActor = nullptr;

            //make its top look towards the rotation
            //or look towards (with own xy pane towards the orthogonal part in any direction (to flip a 2d vector)
            //by 90 dregree, swap x and y component
            if(someActor != nullptr){
                someActor->SetActorLocation(location);

                FVector rotatedNormal(
                    normal.X,
                    normal.Z, // flipped Z and Y to tilt in any direction but 90 degree to pane / pralell
                    normal.Y);
                someActor->SetActorRotation(FVectorUtil::lookAt(location, rotatedNormal));
            }
        }
    }

}



