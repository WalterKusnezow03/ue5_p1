// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "p2/util/FVectorTouple.h"
#include "p2/util/AActorUtil.h"
#include "p2/pathFinding/PathFinder.h"
#include "Components/BoxComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "bezierCurve.h"
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
    setTeam(teamEnum::neutralTeam); //nesecarry for being shot of everyone
    
}

// Called every frame
void AcustomMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// @brief init helper sets the material type for damage reaction
/// @param materialtype 
void AcustomMeshActor::init(materialEnum materialtype){
    //set this var for debree creation
    materialtypeSet = materialtype;

    if(materialtypeSet == materialEnum::glassMaterial){
        setHealth(1);
    }else{
        setHealth(100);
    }
}

// --- derived methods from damageinferface ---

/// @brief will allow custom emsh actors such as destructables and terrain react to damage
/// @param d 
void AcustomMeshActor::takedamage(int d){

    EntityManager *entityManager = worldLevel::entityManager();

    if(entityManager != nullptr){
        //in any case create debree?

        FVector originPoint = GetActorLocation();
        entityManager->createDebree(GetWorld(), originPoint, materialtypeSet);

        // destroy if possible
        if (isDestructable())
        {
            health -= d;
            if(health <= 0){
                health = 100;
            
                SetActorLocation(FVector(0, 0, -10000));

                //not really despawn for now
                AActorUtil::showActor(*this, false);
                AActorUtil::enableColliderOnActor(*this, false);
            
                if(entityManager != nullptr){
                    entityManager->add(this);
                }
            }
        }
    }

    


}

/// @brief allows tha ctor to react to damage from a origin
/// @param d 
/// @param from 
void AcustomMeshActor::takedamage(int d, FVector &from){
    takedamage(d);

    //angle of debree might be calculated from angle to normal for example

}

void AcustomMeshActor::setTeam(teamEnum t){
    this->team = t;
}
teamEnum AcustomMeshActor::getTeam(){
    return team;
}

void AcustomMeshActor::setHealth(int d){
    if(d <= 0){
        health = 1;
        return;
    }
    health = d;
}

// --- mesh actor methods ---

/// @brief will check if the mesh is fully destructable by type
/// @return true false
bool AcustomMeshActor::isDestructable(){

    return materialtypeSet == materialEnum::glassMaterial ||
           materialtypeSet == materialEnum::wallMaterial;
}

/// @brief process a 2D map of local coordinates
/// correct position of the chunk must be set before!
/// @param map 2D vector of LOCAL coordinates!
void AcustomMeshActor::process2DMap(std::vector<std::vector<FVector>> &map){ //nach dem entity manager stirbt die refenz hier!

    
   
    
    //grass
    TArray<FVector> output_layer0;
    TArray<int32> triangles_layer0;

    //stone
    TArray<FVector> output_layer1;
    TArray<int32> triangles_layer1;

    TArray<FVectorTouple> touples; //first arg: center, second: normal

    std::vector<FVector> navMeshAdd;

    FVector originVec(0, 0, 0);

    //iterate over the map and create all triangles by creating the quads from 4 given vertecies
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
                    //get the vertecies
                    FVector vzero = map.at(x).at(y);
                    FVector vone = map.at(x).at(y + 1);
                    FVector vtwo = map.at(x + 1).at(y + 1);
                    FVector vthree = map.at(x + 1).at(y);

                    //add to standard output
                    //buildQuad(vzero, vone, vtwo, vthree, output, newtriangles);

                    FVector normal = FVectorUtil::calculateNormal(vzero, vone, vtwo); //direction obviously
                    if(FVectorUtil::directionIsVertical(normal)){
                        //add to standard output, if direction of normal is vertical, the pane is flat
                        buildQuad(vzero, vone, vtwo, vthree, output_layer0, triangles_layer0);
                    }else{
                        //otherwise the quad should be added to the second
                        //triangle / vertecy array for stone material, more vertical
                        buildQuad(vzero, vone, vtwo, vthree, output_layer1, triangles_layer1);
                    }


                    //calculate center
                    FVector centerLocal = FVectorUtil::calculateCenter(vzero, vone, vtwo);
                    FVector centerWorld = centerLocal + GetActorLocation();
                   

                    // create and add touple to list
                    FVectorTouple t(centerLocal, normal); // first center, then normal
                    touples.Add(t);

                    

                    /**
                     * ADD NODES TO NAV MESH
                     */
                    // only add the normal if the surface is flat

                    //testing only three per chunk, raycasting takes a lot of power
                    if (navMeshAdd.size() <= 6 && FVectorUtil::edgeIsVertical(originVec, normal))
                    {
                        if (navMeshAdd.size() == 0)
                        {
                            navMeshAdd.push_back(centerWorld);
                        }
                        else
                        {
                            // only push nodes 3 meters away from each other -> reduce mesh count
                            FVector &prev = navMeshAdd.back();
                            if (FVector::Dist(prev, centerWorld) >= 300)
                            {
                                navMeshAdd.push_back(centerWorld);
                            }
                        }
                    }
                }catch (const std::exception &e)
                {
                    //this try catch block was just added when debugging can certainly be
                    //kept for safety 
                    DebugHelper::showScreenMessage("mesh actor exception!", FColor::Red);
                }
            }
            
        }
    }


    //process created data and apply meshes and materials

    materialtypeSet = materialEnum::grassMaterial; //might be changed later, left off for particles..

    updateMesh(output_layer0, triangles_layer0, true, 0); //layer 0 grass
    updateMesh(output_layer1, triangles_layer1, true, 1); //layer 1 stone


    //iterate over touples and add foliage based on height and if the pane is flat or vertical
    MeshData mFoliage;
    createFoliage(touples, mFoliage);
    updateMesh(mFoliage, false, 2); //try no normals for trees, layer 2 trees

    if(assetManager *e = assetManager::instance()){

        //grass
        ApplyMaterial(Mesh, e->findMaterial(materialEnum::grassMaterial), 0); //layer 0
        //stone
        ApplyMaterial(Mesh, e->findMaterial(materialEnum::stoneMaterial), 1); //layer 1

        //tree
        ApplyMaterial(Mesh, e->findMaterial(materialEnum::treeMaterial), 2); //layer 2
    
    
    }

    
    
    double StartTime = FPlatformTime::Seconds();
    //add all normal centers to navmesh to allow the bots to move over the terrain
    if(PathFinder *f = PathFinder::instance(GetWorld())){
        FVector offset(0, 0, 70);
        f->addNewNodeVector(navMeshAdd, offset);
    }
    double EndTime = FPlatformTime::Seconds();
    double ElapsedTime = EndTime - StartTime;
    DebugHelper::addTime(ElapsedTime);


    /*
    //debug plot
    for (int i = 0; i < navMeshAdd.size(); i++){
        FVector a = navMeshAdd.at(i);
        FVector b(0, 0, 10);
        b += a;
        DebugHelper::showLineBetween(GetWorld(), a, b, FColor::Red);
    }*/
}




/// @brief processes a 2D and writes the mesh data into the given output object
/// @param map map to process
/// @param outputData output to write
void AcustomMeshActor::process2DMapSimple(
    std::vector<std::vector<FVector>> &map,
    MeshData &outputData
){ 
    //just one layer
    TArray<FVector> output_layer;
    TArray<int32> triangles_layer;

    FVector originVec(0, 0, 0);

    //iterate over the map and create all triangles by creating the quads from 4 given vertecies
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
                    //get the vertecies
                    FVector &vzero = map.at(x).at(y);
                    FVector &vone = map.at(x).at(y + 1);
                    FVector &vtwo = map.at(x + 1).at(y + 1);
                    FVector &vthree = map.at(x + 1).at(y);
                    buildQuad(vzero, vone, vtwo, vthree, output_layer, triangles_layer);

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

    outputData.clearMesh();
    outputData.setVertecies(MoveTemp(output_layer)); //r value reference must be moved
    outputData.setTriangles(MoveTemp(triangles_layer));
    outputData.calculateNormals();
}



void AcustomMeshActor::updateMesh(
    TArray<FVector> &newvertecies,
    TArray<int32> &newtriangles,
    bool createNormals //makes the texture appear flat if not enabled
){
    updateMesh(newvertecies, newtriangles, createNormals, 0);
}



/// @brief updates the mesh for the actor
/// @param newvertecies vertecies to set -> OWNER SHIP WILL BE TAKEN
/// @param newtriangles triangles for the vertecies -> OWNER SHIP WILL BE TAKEN
/// @param createNormals to create normals or not
void AcustomMeshActor::updateMesh(
    TArray<FVector> &newvertecies,
    TArray<int32> &newtriangles,
    bool createNormals, //makes the texture appear flat if not enabled
    int layer
)
{

    //find from map
    MeshData *data;
    if (meshLayersMap.find(layer) != meshLayersMap.end()){
        //find meshData from map by reference
        data = &meshLayersMap[layer]; //hier mit eckigen klammern weil .find ein iterator ist
    }else{
        meshLayersMap[layer] = MeshData(); //add
        data = &meshLayersMap[layer]; //assign the pointer as needed
    }

    if(data == nullptr){
        return; //an issue occured return
    }

    //CLEAR ALL FROM PREVIOUS DATA, EVERYTHING - to prevent any weird issues.
    data->clearMesh();

    //refresh data 
    //use MoveTemp to make the r value reference (from left value to right value)
    data->setVertecies(MoveTemp(newvertecies));
    data->setTriangles(MoveTemp(newtriangles));
    if(createNormals){
        data->calculateNormals(); //calculate the normals to fix flat lighting issue
    }else{
        data->clearNormals();
    }

    // Create the mesh section
    //int sectionIndex = 0; //auch bei den materials, hier das erste argument, merken
    if(Mesh != nullptr){
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
        Mesh->ClearMeshSection(layer);
        Mesh->CreateMeshSection(
            layer, 
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


    //enable if was disabled!
    AActorUtil::showActor(*this, true);
    AActorUtil::enableColliderOnActor(*this, true);
}




/// @brief updates a mesh layer given on a mesh data object (which will be deep copied)
/// @param otherMesh 
/// @param createNormals 
/// @param layer 
void AcustomMeshActor::updateMesh(MeshData otherMesh, bool createNormals, int layer){

    meshLayersMap[layer] = otherMesh; //assign operator is overriden

    MeshData *data = nullptr;
    if (meshLayersMap.find(layer) != meshLayersMap.end()){
        //find meshData from map by reference
        data = &meshLayersMap[layer]; //hier mit eckigen klammern weil .find ein iterator ist
    }

    if(data != nullptr && Mesh != nullptr){
        data->clearNormals();
        if(createNormals){
            data->calculateNormals();
        }
        

        Mesh->ClearMeshSection(layer);
        Mesh->CreateMeshSection(
            layer, 
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
    //enable if was disabled!
    AActorUtil::showActor(*this, true);
    AActorUtil::enableColliderOnActor(*this, true);


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
    TArray<FVector> newNormals;
    // bottom
    // flipped 180 degree?
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
    

    updateMesh(output, newtriangles, false);
    if(material != nullptr){
        ApplyMaterial(Mesh, material);
    }else{
        //find wall material if none was provided / nullptr
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
        TArray<FVector> newNormals;

        //a b c und d sollten richtig herum gedreht sein wenn man abc und d bildet
        buildQuad(a, b, c, d, output, newtriangles);

        //flipped 180 degree?
        buildQuad(a, d, c, b, output, newtriangles);


        updateMesh(output, newtriangles, false);
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


/// @brief applys a material to the whole component (slot 0 by default)
/// @param ProceduralMeshComponent 
/// @param Material 
void AcustomMeshActor::ApplyMaterial(UProceduralMeshComponent* ProceduralMeshComponent, UMaterial* Material) {
    ApplyMaterial(ProceduralMeshComponent, Material, 0);
}

/// @brief applys a material to the whole component at passed index slot
/// @param ProceduralMeshComponent mesh to apply for
/// @param Material material to set
/// @param layer layer to apply for / index
void AcustomMeshActor::ApplyMaterial(
    UProceduralMeshComponent* ProceduralMeshComponent, 
    UMaterial* Material,
    int layer
) {
	if (ProceduralMeshComponent != nullptr && Material != nullptr) {
		// Apply the material to the first material slot (index 0) of the procedural mesh
		ProceduralMeshComponent->SetMaterial(layer, Material);
	}
}






/// @brief create foliage and append it to the output mesh data, the output mesh data will
/// get its position from the actor. The touples expected to be in local coordinate system
/// @param touples lcoation and normal in a touple
/// @param outputAppend for example a terrain mesh to create trees on
void AcustomMeshActor::createFoliage(TArray<FVectorTouple> &touples, MeshData &outputAppend){
    // iterate over touples
    // determine normal angle and apply foliage, rocks, trees accordingly
    if(touples.Num() < 1){
        return;
    }

    int created = 0;

    //if normal faces towards up: flat area, create something
    for(FVectorTouple &t : touples){

        FVector &location = t.first();
        FVector &normal = t.second();

        bool facingUpwards = FVectorUtil::directionIsVertical(normal);
        if(facingUpwards){
            
            MeshData mData = createTree(10, 1); //need to apply offset!

            //apply offset
            TArray<FVector> &vertecies = mData.getVerteciesRef();
            for (int i = 0; i < vertecies.Num(); i++){
                vertecies[i] += location;
            }

            outputAppend.append(mData);
            
            created++;
            if(created > 2){
                break;
            }
        }
    }

}





/// @brief process touples with
/// @param touples vector of touples with each: (first: center of pane with correct offset, second: normal of pane)
void AcustomMeshActor::createFoliage(TArray<FVectorTouple> &touples)
{

    // iterate over touples
    // determine normal angle and apply foliage, rocks, trees accordingly
    if(touples.Num() < 1){
        return;
    }

    int created = 0;

    MeshData mDataAll;
    createFoliage(touples, mDataAll);

    FVector locationBase = GetActorLocation();

    //Apply data
    if (EntityManager *e = worldLevel::entityManager()){
        AcustomMeshActor *meshActor = e->spawnAcustomMeshActor(GetWorld(), locationBase);
        if(meshActor != nullptr){
            meshActor->SetActorLocation(locationBase);


            meshActor->updateMesh(
                mDataAll,
                false,
                0
            );
            if(assetManager *a = assetManager::instance()){
                meshActor->ApplyMaterial(meshActor->Mesh, a->findMaterial(materialEnum::wallMaterial));
            }

            //meshActor->AttachToActor(this);
        }
    }
}

/// @brief will replace the actor and split it (int terms of bounds) and apply an material
/// will not use original mesh, just the bounds
/// @param actor actor to replace with splitted mesh
/// @param bottomCenter bottom center of the actor, very important, do not ignore
/// @param cmTile each tile to be cm wide and high
/// @param material material to set for the mesh
void AcustomMeshActor::splitAndreplace(
    AActor *actor,
    FVector &bottomCenter,
    int cmTile,
    materialEnum materialType
)
{
    
    UMaterial *material = nullptr;
    assetManager *am = assetManager::instance();
    if(am != nullptr){
        material = am->findMaterial(materialType);
    }

    if(actor != nullptr && material != nullptr){

        int xBound = 0;
        int yBound = 0;
        int zBound = 0;
        AActorUtil::calculateActorBounds(actor, xBound, yBound, zBound);

        FRotator rotationCopy = actor->GetActorRotation();

        //create a mesh where the pieces are almost a similar side viewed from the larger side
        //like consitent quads

        EntityManager *eM = worldLevel::entityManager();
        if (eM != nullptr)
        {
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
            

            //DEBUGGING NEEDED

            //new: symetrical splitting, randomize offset, create meshes
            std::vector<std::vector<FVector>> splitted;
            for (int i = 0; i <= tilesXAxis; i++)
            {
                std::vector<FVector> positions;
                for (int j = 0; j <= tilesZAxis; j++)
                {
                    //create the grid properly
                    FVector pos = i * side + j * up;
                    positions.push_back(pos);
                }
                splitted.push_back(positions);
            }

            //random offsets
            for (int i = 1; i < splitted.size() - 1; i++)
            {
                for (int j = 1; j < splitted[i].size() - 1; j++){ //only inner vertecies to offset
                    // scale from 0 to 1
                    FVector offset = side * FVectorUtil::randomFloatNumber(-1.0f, 1.0f) * 0.5f;
                    splitted[i][j] += offset;
                }
            }
            //create cubes from touples like 2d map
            for (int i = 0; i < splitted.size() - 1; i++)
            {
                for (int j = 0; j < splitted[i].size() - 1; j++){

                    FVector center = anchor + splitted[i][j];
                    AcustomMeshActor *newActor = eM->spawnAcustomMeshActor(actor->GetWorld(), center);
                    if (newActor != nullptr){
                        /*
                        1  2

                        0  3
                        */
                        FVector &t0_base = splitted[i][j];
                        FVector t0(0, 0, 0);
                        FVector t1 = splitted[i][j+1] - t0_base;
                        FVector t2 = splitted[i+1][j+1] - t0_base;
                        FVector t3 = splitted[i+1][j] -t0_base;

                        FVector t0a = t0 + extension;
                        FVector t1a = t1 + extension;
                        FVector t2a = t2 + extension;
                        FVector t3a = t3 + extension;

                        DebugHelper::showLineBetween(actor->GetWorld(), center + t0, center + t1);
                        DebugHelper::showLineBetween(actor->GetWorld(), center + t1, center + t2);
                        DebugHelper::showLineBetween(actor->GetWorld(), center + t2, center + t3);
                        DebugHelper::showLineBetween(actor->GetWorld(), center + t3, center + t0);

                        // init on begin!
                        newActor->init(materialType);

                        // apply mesh
                        // is correct like this, do not touch
                        newActor->createCube(
                            t0, t1, t2, t3,     //bottom quad clw
                            t0a, t1a, t2a, t3a, //top quad clw
                            material
                        );
                        
                    }
                }
            }

            /*
            // old symetrical splitting
            for (int i = 0; i < tilesXAxis; i++)
            {
                for (int j = 0; j < tilesZAxis; j++)
                {

                    FVector innerAnchor = i * side + j * up;
                    FVector vert0(0, 0, 0);
                    FVector vert1 = vert0 + up;
                    FVector vert2 = vert0 + up + side;
                    FVector vert3 = vert0 + side;

                    FVector center = anchor + innerAnchor;

                    // create new mesh / cube
                    AcustomMeshActor *newActor = eM->spawnAcustomMeshActor(actor->GetWorld(), center);

                    if (newActor != nullptr)
                    {

                        FVector vert0a = vert0 + extension;
                        FVector vert1a = vert1 + extension;
                        FVector vert2a = vert2 + extension;
                        FVector vert3a = vert3 + extension;

                        // init on begin!
                        newActor->init(materialType);

                        // apply mesh
                        // is correct like this, do not touch
                        newActor->createCube(
                            vert0, vert1, vert2, vert3,     // bottom quad clw
                            vert0a, vert1a, vert2a, vert3a, // top quad clw
                            material);
                    }
                }
            }*/
        }
    }
}






/// @brief creates a random curve and an extruded bezier from there, will be symetrical
/// @param sizeMeters 
/// @param thicknessMeters 
/// @return 
MeshData AcustomMeshActor::createTree(int sizeMeters, float thicknessMeters){
    MeshData outmeshData;
    bezierCurve s;

    //create anchors and curve
    int nodesToMake = 3;
    int einheitsValue = (sizeMeters / nodesToMake);
    std::vector<FVector2D> nodes;
    for (int i = 0; i < nodesToMake; i++)
    {
        FVector2D b(
            einheitsValue * i,//step along x axis 
            FVectorUtil::randomNumber(0, sizeMeters / 3) //some random number
        );
        b *= 100;
        nodes.push_back(b);
        if(i == 0){
            b.Y += 10;
            nodes.push_back(b);
        }
    }

    TVector<FVector2D> out;
    int stepsPerEinheit = 4;
    s.calculatecurve(nodes, out, einheitsValue, stepsPerEinheit);

    

    //extrude from points
    int t = (thicknessMeters * 100) / 4; //thickness
    std::vector<FVector> extrudeDirs = {
        FVector(-t, -t, 0),
        FVector(-t, t, 0),
        FVector(t, t, 0),
        FVector(t, -t, 0),
        FVector(-t, -t, 0) //create circle, reconnect to first! (?)
    };

    //create 2d mesh instead which can be wrapped like a 2d mesh but vertically
    
    std::vector<std::vector<FVector>> meshWrap;
    for (int i = 0; i < out.size(); i++)
    {
        FVector2D &upper = out[i];
        FVector UpperTo3D(
            upper.Y,
            0,
            upper.X
        );
        

        //extrude needed and order to create a 2d mesh properly, lower, upper, around the mesh
        std::vector<FVector> higherRing;
        for (int j = 0; j < extrudeDirs.size(); j++)
        {
            //upper, lower is prev upper, already added
            FVector newUpper = UpperTo3D + extrudeDirs[j];
            higherRing.push_back(newUpper);
        }

        meshWrap.push_back(higherRing);
    }

    process2DMapSimple(meshWrap, outmeshData);
    return outmeshData;
}