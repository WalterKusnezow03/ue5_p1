// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/util/FVectorTouple.h"
#include "p2/util/AActorUtil.h"
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
    setTeam(teamEnum::none);
}

// Called every frame
void AcustomMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// --- derived methods from damageinferface ---

/// @brief will allow custom emsh actors such as destructables and terrain react to damage
/// @param d 
void AcustomMeshActor::takedamage(int d){

    DebugHelper::showScreenMessage("debug mesh damage");
    // handle terrain or other type here
    if(d > 100){

        //some sort of impulse if greater than a 100 for example
        //or another method just for explosive damage
        //unklar!
        //mehr damage wäre evt einfacher aber dadurch ist die richtung auch unklar
        //das interface muss wohl erweitert werden. 

    }
}

void AcustomMeshActor::takedamage(int d, FVector &from){
    takedamage(d);

    //react to impulse here ----> testing needed!
    if(Mesh != nullptr){
        // Enable physics simulation
        Mesh->SetSimulatePhysics(true);
        FVector Impulse = 1000 * (GetActorLocation() - from).GetSafeNormal(); //AB = B - A
        Mesh->AddImpulse(Impulse);
    }

}

void AcustomMeshActor::setTeam(teamEnum t){
    this->team = t;
}
teamEnum AcustomMeshActor::getTeam(){
    return team;
}

// --- mesh actor methods ---


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
                    //this try catch block was just added when debugging can certainly be
                    //kept for safety 
                    DebugHelper::showScreenMessage("mesh actor exception!", FColor::Red);
                }
            }
            
        }
    }


    //process created data
    updateMesh(output, newtriangles);
    if(assetManager *e = assetManager::instance()){
        ApplyMaterial(Mesh, e->findMaterial(materialEnum::grassMaterial));
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


/// @brief creates a cube from 4 vertecies and a material
/// expecting the vertecies to be already ordered correctly in clockwise order from a to d!
/// expecting a to d to be the bottom side and the direction to be positive!
/// @param a a0
/// @param b b1
/// @param c c2
/// @param d d3
/// @param dir direction to extend
/// @param cmheight height of the shape / direction length
void AcustomMeshActor::createCube(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &dir,
    int cmheight,
    UMaterial *material
){
    if(material != nullptr){

        dir = dir.GetSafeNormal() * cmheight;

        FVector a1 = a + dir;
        FVector b1 = b + dir;
        FVector c1 = c + dir;
        FVector d1 = d + dir;

        createCube(a, b, c, d, a1, b1, c1, d1, material);
    }
}

/// @brief expect a -d to be clockwise bottom quad and a1-d1 to be clockwise top quad
/// @param a 
/// @param b 
/// @param c 
/// @param d 
/// @param a1 
/// @param b1 
/// @param c1 
/// @param d1 
void AcustomMeshActor::createCube(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    FVector &a1, 
    FVector &b1,
    FVector &c1,
    FVector &d1,
    UMaterial *material
){

    TArray<FVector> output;
    TArray<int32> newtriangles;
    //bottom
    //flipped 180 degree?
    buildQuad(a, d, c, b, output, newtriangles);

    //top
    //a b c und d sollten richtig herum gedreht sein wenn man abc und d bildet
    buildQuad(a1, b1, c1, d1, output, newtriangles);

    //sides
    //must be reverse winding order (ccl)
    //instead of 0123 -> 3210 to be flipped correctly!
    buildQuad(b, b1, a1, a, output, newtriangles); //correct, must be reverse winding order (ccl)
    buildQuad(c, c1, b1, b, output, newtriangles); 
    buildQuad(d, d1, c1, c, output, newtriangles);
    buildQuad(a, a1, d1, d, output, newtriangles);
    

    updateMesh(output, newtriangles);
    if(material != nullptr){
        ApplyMaterial(Mesh, material);
    }else{
        if(assetManager *e = assetManager::instance()){
            ApplyMaterial(Mesh, e->findMaterial(materialEnum::wallMaterial));
        }
    }
}





/// @brief creates a two sided quad from 4 vertecies and a material
/// expecting the vertecies to be already ordered correctly in clockwise order from a to d!
/// @param a a0
/// @param b b1
/// @param c c2
/// @param d d3
/// @param material material to be applied, must not be nullptr
void AcustomMeshActor::createTwoSidedQuad(
    FVector &a, 
    FVector &b,
    FVector &c,
    FVector &d,
    UMaterial *material
){
    if(material != nullptr){
        TArray<FVector> output;
        TArray<int32> newtriangles;

        //a b c und d sollten richtig herum gedreht sein wenn man abc und d bildet
        buildQuad(a, b, c, d, output, newtriangles);

        //flipped 180 degree?
        buildQuad(a, d, c, b, output, newtriangles);


        updateMesh(output, newtriangles);
        ApplyMaterial(Mesh, material);
    }
}






/// @brief build a quad out of two triangles! Important otherwise unfixable issues are in the mesh
/// @param a 
/// @param b 
/// @param c 
/// @param d 
/// @param output 
/// @param trianglesOutput 
void AcustomMeshActor::buildQuad(
    FVector &a, 
    FVector &b, 
    FVector &c, 
    FVector &d, 
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

                b--c
                |  |
                a<-d
    */

}



/// @brief all quads MUST BE BUILD out of TRIANGLES, OTHERWISE MANY BUGS OCCUR!
/// @param a corner 0
/// @param b corner 1
/// @param c corner 2
/// @param output output to append in
/// @param trianglesOutput triangle int32 as nums saved in here, also appended
void AcustomMeshActor::buildTriangle(
    FVector &a, 
    FVector &b, 
    FVector &c,
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
	if (ProceduralMeshComponent != nullptr && Material != nullptr) {
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








/// @brief will replace the actor and split it (int terms of bounds) and apply an material
/// will not use original mesh, just the bounds
/// @param actor actor to replace with splitted mesh
/// @param bottomCenter bottom center of the actor, very important, do not ignore
/// @param cmTile each tile to be cm wide and high
/// @param material material to set for the mesh
void AcustomMeshActor::splitAndreplace(AActor *actor, FVector &bottomCenter, int cmTile, UMaterial *material)
{
    if(actor != nullptr && material != nullptr){

        int xBound = 0;
        int yBound = 0;
        int zBound = 0;
        AActorUtil::calculateActorBounds(actor, xBound, yBound, zBound);

        FRotator rotationCopy = actor->GetActorRotation();

        //create a mesh where the pieces are almost a similar side viewed from the larger side
        //like consitent quads

        EntityManager *eM = EntityManager::instance();
        if(eM != nullptr){
            //bottom left corner
            FVector anchor = bottomCenter;
            anchor.X -= xBound / 2; //bottom left now (bounds adjusted half way obviosuly)
            anchor.Y -= yBound / 2;

            FVector up(0, 0, cmTile); //always in up direction for now
            FVector side = (xBound > yBound) ? FVector(cmTile, 0, 0) : FVector(0, cmTile, 0); //iterate along longer
            FVector extension = (xBound > yBound) ? FVector(0, yBound, 0) : FVector(xBound, 0, 0);  //90 degree to longer
            

            
            float tilesXAxis = (xBound > yBound) ? xBound / cmTile : yBound / cmTile;
            float tilesZAxis = zBound / cmTile;
            if(tilesXAxis < 1){
                side *= tilesXAxis; //scale accordinly down!
                tilesXAxis = 1; // set to one to still iterate once
            }
            if(tilesZAxis < 1){
                up *= tilesZAxis; //scale accordinly down!
                tilesZAxis = 1; // set to one to still iterate once
            }
            


            for (int i = 0; i < tilesXAxis; i++){
                for (int j = 0; j < tilesZAxis; j++){
                    
                    FVector innerAnchor = i * side + j * up;
                    FVector vert0(0,0,0);
                    FVector vert1 = vert0 + up;
                    FVector vert2 = vert0 + up + side;
                    FVector vert3 = vert0 + side;

                    FVector center = anchor + innerAnchor; 

                    //create new mesh / cube
                    AcustomMeshActor *newActor = eM->spawnAcustomMeshActor(actor->GetWorld(), center);

                    if(newActor != nullptr){
                        
                        DebugHelper::showLineBetween(actor->GetWorld(), anchor, FVector(0,0,0), FColor::Green);

                        FVector vert0a = vert0 + extension;
                        FVector vert1a = vert1 + extension;
                        FVector vert2a = vert2 + extension;
                        FVector vert3a = vert3 + extension;

                        //apply mesh
                        //is correct like this, do not touch
                        newActor->createCube(
                            vert0, vert1, vert2, vert3, //bottom quad clw
                            vert0a, vert1a, vert2a, vert3a, //top quad clw
                            material
                        );

                    }

            
                }
            }
        }

        
    }
}