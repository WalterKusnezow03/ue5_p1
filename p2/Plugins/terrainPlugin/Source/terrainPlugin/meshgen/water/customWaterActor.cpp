// Fill out your copyright notice in the Description page of Project Settings.


#include "customWaterActor.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "GameCore/MeshGenBase/lodHelper/LodCheckContainer.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"
#include "ripple.h"


AcustomWaterActor::AcustomWaterActor() : AcustomMeshActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    meshInited = false;
    setTeam(teamEnum::neutralTeam);
}

void AcustomWaterActor::BeginPlay(){
    Super::BeginPlay();
    meshInited = false;
    setTeam(teamEnum::neutralTeam);
}


void AcustomWaterActor::Tick(float DeltaTime){
    Super::Tick(DeltaTime);


    if(!meshInited){
        return;
    }

    //cpue sided vertex offset is only happening
    //if the player is in range.
    if (playerIsInRenderRange()){
        //updateRunningTime(DeltaTime);
        TickRipples(DeltaTime); //tick ripples before vertex shader to already modify mesh
        vertexShader();
    }
}


void AcustomWaterActor::createWaterPane(
    UWorld *world, 
    FVector location,
    int scaleMeters
){
    if(world == nullptr){
        return;
    }
    if(scaleMeters < 1){
        return;
    }

    float fVertexCount = (float) scaleMeters / DEFAULT_DISTANCE_BETWEEN_VERTECIES;
    int paneCount = fVertexCount / MAX_VERTEXCOUNT;
    if(paneCount == 0){ //below max size, do 1
        paneCount = 1;
    }

    //int offsetOnAxis = MAX_VERTEXCOUNT * DEFAULT_DISTANCE_BETWEEN_VERTECIES;

    int verticesPerPane = fVertexCount / paneCount;
    int offsetOnAxis = verticesPerPane * DEFAULT_DISTANCE_BETWEEN_VERTECIES;

    //DebugHelper::logMessage("DEBUGSIZE OF WATER VERTEXCOUNT", vertexCount);
    //DebugHelper::logMessage("DEBUGSIZE OF WATER PANECOUNT", paneCount);

    FVector offsetVector(0,0,100);
    offsetVector += location;

    for (int i = 0; i < paneCount; i++){
        for (int j = 0; j < paneCount; j++){

            FVector finalLocation = offsetVector;
            finalLocation.X += offsetOnAxis * i;
            finalLocation.Y += offsetOnAxis * j;

            FRotator rotation;
            FActorSpawnParameters params;
            AcustomWaterActor *SpawnedActor = world->SpawnActor<AcustomWaterActor>(
                AcustomWaterActor::StaticClass(),
                finalLocation,
                FRotator::ZeroRotator,
                params
            );
            if(SpawnedActor != nullptr){
                SpawnedActor->createWaterPane(
                    verticesPerPane, 
                    DEFAULT_DISTANCE_BETWEEN_VERTECIES
                );
            }
        }
    }
}




/// @brief 
/// @param sizeX in vertecies
/// @param sizeY in vertecies
/// @param detail detail between vertecies (in cm)
void AcustomWaterActor::createWaterPane(int vertexCountIn, int detail){
    if(meshInited){
        return;
    }

    vertexCountIn = std::abs(vertexCountIn);
    if(vertexCountIn <= 2){
        vertexCountIn = 3;
    }
    /*if(vertexCountIn > MAX_VERTEXCOUNT){
        vertexCountIn = MAX_VERTEXCOUNT;
    }*/

    vertexcountX = vertexCountIn;
    vertexcountY = vertexCountIn;

    //VERY IMPORTANT
    BottomLeft = FVector(0,0,0);
	TopLeft = FVector(0, vertexCountIn * detail, 0);
	BottomRight = FVector(vertexCountIn * detail,0,0);
	TopRight = FVector(vertexCountIn * detail,vertexCountIn * detail,0);

    ownHalfSize = (vertexCountIn * detail) / 2.0f;

    MeshData &waterMesh = findMeshDataReference(
        materialEnum::waterMaterial,
        ELod::lodNear
    );

    for (int i = 0; i < vertexcountX; i++){
        for (int j = 0; j < vertexcountY; j++){
            int xOffset = i * detail;
            int yOffset = j * detail;
            /*
            1 2
            0 3
            */
            FVector v0 = FVector(xOffset, yOffset, 0);
            FVector v1 = FVector(xOffset, yOffset + detail, 0);
            FVector v2 = FVector(xOffset + detail, yOffset + detail, 0);
            FVector v3 = FVector(xOffset + detail, yOffset, 0);

            waterMesh.appendEfficent(v0, v1, v2, v3);
        }
    }
    waterMesh.calculateNormals();

    ReloadMeshAndApplyAllMaterials();
    meshInited = true;
    originalVertecies = waterMesh.getVerteciesRefConst();

    //exclude this for bone controller raycast
    /*
    EntityManager *entityManagerPointer = AworldLevel::entityManager();
    if(entityManagerPointer){
        entityManagerPointer->addActorToIgnoredAllParams(this);
    }*/


    //update collsion params for this mesh after it was setup
    UProceduralMeshComponent *thisMesh = meshComponentPointer();
    if (thisMesh)
    {
        thisMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        thisMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
        thisMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

        /*//set custom depth for screen space refraction
        int depthPass = 1;
        thisMesh->SetRenderCustomDepth(true);
        thisMesh->SetCustomDepthStencilValue(depthPass);
        thisMesh->MarkRenderStateDirty();

        int32 Value = thisMesh->CustomDepthStencilValue;
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("AcustomWaterActor::setupDepthPass %d"), Value
            )
        );*/
    }

    distanceBetweenVerteciesSaved = detail;
}

MeshData& AcustomWaterActor::findMeshDataReference(
    materialEnum mat,
    ELod lod
){
    return Super::findMeshDataReference(
        mat,
        lod,
        true // mesh
    );
}

UProceduralMeshComponent* AcustomWaterActor::meshComponentPointer(){
    if(Mesh){
        return Mesh;
    }
    return nullptr;
}


void AcustomWaterActor::vertexShader(){
    MeshData &waterMesh = findMeshDataReference(
        materialEnum::waterMaterial,
        ELod::lodNear
    );

    int layer = layerByMaterialEnum(materialEnum::waterMaterial);

    FVector locationOfPlayer = PlayerInfo::playerLocation();

    //raycast is not blocked by water
    UProceduralMeshComponent *thisMesh = meshComponentPointer();
    if(thisMesh){
        MeshData &ref = waterMesh;
        TArray<FVector> &vertecies = ref.getVerteciesRef();

        //old working, linear: bad
        
        FVector actorLocation = GetActorLocation();
        for (int i = 0; i < vertecies.Num(); i++)
        {
            if(i >= 0 && i < vertecies.Num()){
                FVector &vertex = vertecies[i];

                if(!isAtLockedAxis(vertex)){
                    
                    //cant be deprecated, handled in shader, but needed for 
                    //ripples right now
                    resetVertexShadignFor(vertex, i, vertecies.Num());
                    applyShaderToVertex(vertex);
                    applyWaterRippleOffset(vertex, actorLocation);
                }else{
                    //debug draw shows it should be fine, somewhere the flag gets resettet
                    /*
                    DebugHelper::showLineBetween(
                        GetWorld(),
                        GetActorLocation() + vertex,
                        GetActorLocation() + vertex + FVector(0, 0, 1000),
                        FColor::Orange,
                        0.1f
                    );*/
                    resetVertexShadignFor(vertex);
                }
            }
            
        }

        refreshMesh(*thisMesh, ref, layer);
    }
}


void AcustomWaterActor::resetVertexShadignFor(FVector &vertex, int index, int all){
    if(index >= 0 && index < originalVertecies.Num()){
        FVector copy = originalVertecies[index];
        vertex = copy;
        resetVertexShadignFor(vertex);
        return;
    }

    //horizontal gibt es 
    //vertikale columns
    int numVerticesY = vertexcountY + 1;
    int xPos = (index / numVerticesY);
    int yPos = index % numVerticesY;

    resetVertexShadignFor(vertex);
    vertex.X = xPos * distanceBetweenVerteciesSaved;
    vertex.Y = yPos * distanceBetweenVerteciesSaved;
}

void AcustomWaterActor::resetVertexShadignFor(FVector &other){
    other.Z = 0.0f;
}




///@brief will tell whether a vertex should be moved or not
bool AcustomWaterActor::isAtLockedAxis(FVector &other){
    //check for axis
    int buffer = 100;
    if (topAxisLocked)
    {
        if(other.Y + buffer >= TopRight.Y){
            return true;
        }
    }
    if(bottomAxisLocked){
        if(other.Y - buffer <= BottomRight.Y){
            return true;
        }
    }
    if(rightAxisLocked){
        if(other.X + buffer >= BottomRight.X){
            return true;
        }
    }
    if(leftAxisLocked){
        if(other.X - buffer <= BottomLeft.X){
            return true;
        }
    }
    return false;
}

#include "terrainPlugin/meshgen/water/waveShaders/WaveProperty.h"

/// @brief apply vertex shader to the given vertex
/// @param vertex vertex to move
void AcustomWaterActor::applyShaderToVertex(FVector &vertex){
    /*
    shader.SetWaveLength(1000.0f); // must be greater than vertex distance
    shader.SetWaveSpeed(0.5f);
    shader.SetAmplitude(20.0f);
    shader.SetWaveSteepNess(3.0f);

    shader.SetWaveLength(800.0f); // must be greater than vertex distance
    shader.SetWaveSpeed(0.5f);
    shader.SetAmplitude(40.0f);
    shader.SetWaveSteepNess(0.1f);

    shader.UpdateShaderRunningTime(shaderRunningTime);
    shader.applyShaderToVertex(vertex, actorLocation);
    */

    FVector actorLocation = GetActorLocation();

    WaveProperty property0;
    WaveProperty property1;
    WaveProperty property2;

    FVector2D dir(1, 0);
    property0.SetWaveLength(1000.0f); // must be greater than vertex distance
    property0.SetWaveSpeed(0.5f);
    property0.SetAmplitude(20.0f);
    property0.SetWaveSteepNess(0.2f);
    property0.SetWaveDirection(dir);

    dir = FVector2D(4, 1);
    property1.SetWaveLength(500.0f); // must be greater than vertex distance
    property1.SetWaveSpeed(0.5f);
    property1.SetAmplitude(30.0f);
    property1.SetWaveSteepNess(0.3f);
    property1.SetWaveDirection(dir);

    dir = FVector2D(1, 2);
    property2.SetWaveLength(200.0f); // must be greater than vertex distance
    property2.SetWaveSpeed(1.0f);
    property2.SetAmplitude(10.0f);
    property2.SetWaveSteepNess(0.7f);
    property2.SetWaveDirection(dir);

    TArray<WaveProperty> properties = {property0, property1, property2};

    //mehrere wellen müssen addiertw erden
    //die steilere winkel haben
    //damit die spitzen entstehen!

     

    shader.UpdateShaderRunningTime(shaderRunningTime);
    shader.applyShadersToVertex(vertex, actorLocation, properties);



}


void AcustomWaterActor::resetAllShaderOffsets(){
    UProceduralMeshComponent *thisMesh = meshComponentPointer();
    if(thisMesh){

        MeshData &waterMesh = findMeshDataReference(
            materialEnum::waterMaterial,
            ELod::lodNear
        );
    
        int layer = layerByMaterialEnum(materialEnum::waterMaterial);
    
        TArray<FVector> &verteciesReference = waterMesh.getVerteciesRef();
        for (int i = 0; i < verteciesReference.Num(); i++){
            resetVertexShadignFor(verteciesReference[i]);
        }

        refreshMesh(*thisMesh, waterMesh, layer);
    }
}



void AcustomWaterActor::refreshMesh(
    UProceduralMeshComponent& meshComponent,
    MeshData &other,
    int layer
){
    if(meshInited){
        Super::refreshMesh(meshComponent, other, layer);
    }
}




/**
 * -- damage interaction --
 */

void AcustomWaterActor::takedamage(FCustomHitResult &result){
    if(result.HasHitPoint()){
        addNewRipple(result.HitPoint());
    }
}


/**
 * 
 * -- ripple index management --
 * 
*/
void AcustomWaterActor::TickRipples(float DeltaTime){
    std::vector<int> markedForRemoval;
    for (int i = 0; i < rippleVecSize; i++)
    {
        if(rippleIndexIsValid(i)){
            ripple &current = rippleVector[i];
            current.Tick(DeltaTime);
            if (current.timeExceeded())
            {
                markedForRemoval.push_back(i);
            }
        }
    }

    for (int i = 0; i < markedForRemoval.size(); i++){
        removeRippleAtIndex(markedForRemoval[i]);
    }
}

void AcustomWaterActor::applyWaterRippleOffset(FVector &vertex, FVector &actorLocation){
    float offsetSum = 0.0f;
    for (int i = 0; i < rippleVecSize; i++)
    {
        if(rippleIndexIsValid(i)){
            ripple &current = rippleVector[i];
            current.changeHeightBasedOnDistance(vertex, actorLocation);
        }
    }
}

///@brief creates a new or reuses an old ripple object from the local object pool
void AcustomWaterActor::addNewRipple(FVector &location){
    if(rippleVector.size() == 0 || rippleVecSize >= rippleVector.size()){
        rippleVector.push_back(ripple(location, ownHalfSize)); //ownHalfSize for max radius
        rippleVecSize = rippleVector.size();
    }
    
    if(rippleVecSize < rippleVector.size()){
        int last = rippleVecSize;
        rippleVector[last].init(location, ownHalfSize * 0.5f); //reuse object
        rippleVecSize++;
    }

}


bool AcustomWaterActor::rippleIndexIsValid(int index){
    return index >= 0 && index < rippleVecSize && index < rippleVector.size();
}

void AcustomWaterActor::removeRippleAtIndex(int index){
    if(rippleVector.size() == 0){
        rippleVecSize = 0;
        return;
    }

    if(rippleIndexIsValid(index)){
        int preLast = rippleVecSize - 1;
        if(preLast == index){
            rippleVecSize--;
            return;
        }

        if(rippleIndexIsValid(preLast)){
            //copy "back" to index and hide with seperate end pointer
            rippleVector[index] = rippleVector[preLast];
            rippleVecSize--;
        }
    }
}




/**
 * helper player distance
 */

int AcustomWaterActor::Meters(int meters){
    return meters * 100.0f;
}

bool AcustomWaterActor::playerIsInRenderRange(){

    FVector playerLook = PlayerInfo::playerLookDir();
    FVector locationOfPlayer = PlayerInfo::playerLocation();


    FVector actorLocation = GetActorLocation();
    LodCheckContainer checkContainer;
    checkContainer.modifyUpperDistanceLimitFor(ELod::lodNear, Meters(50)); //50
    checkContainer.checkLod(actorLocation, locationOfPlayer);
    SetActorHiddenInGame(checkContainer.hideActorByLod()); //if far, hide


    //remove vertex displacement if changed to middle
    ELod lodResult = checkContainer.lod();
    ELod prevLod = latestLodMeasured;
    latestLodMeasured = lodResult;

    if(lodResult == ELod::lodFar){
        if(prevLod != lodResult){
            resetAllShaderOffsets(); //remove vertex displacement ONCE
        }
        return false;
    }

    //check for edge case near to middle
    if(lodResult == ELod::lodNear){
        
        if(checkContainer.lodWasEdgeCaseToNextLod()){ //was very near to middle case

            //do vertex alignment at end to have none
            
            
            //lock according rows to player look to actor location
            lockOuterAxisBasedOn(
                locationOfPlayer,
                playerLook
            );
        }else{
            unlockAllAxis();
        }

        return true;   
    }

    return false;
}


/**
 * locking axis
 */

void AcustomWaterActor::unlockAllAxis(){
    topAxisLocked = false;
	bottomAxisLocked = false;
	leftAxisLocked = false;
	rightAxisLocked = false;
}

void AcustomWaterActor::lockOuterAxisBasedOn(
    FVector &playerLocation,
    FVector &playerLookDir
){
    unlockAllAxis();
    FVector playerInLocalSpace = playerLocation - GetActorLocation();
    playerInLocalSpace.Z = 0.0f;
    playerLookDir = playerLookDir.GetSafeNormal();
    playerLookDir.Z = 0.0f;


    //koordinaten eindrehen, dann messen
    MMatrix playerRotator = MMatrix::createRotatorFrom(playerLookDir);
    FVector topRightRotated = playerRotator * TopRight;
    FVector bottomRightRotated = playerRotator * BottomRight;
    FVector topLeftRotated = playerRotator * TopLeft;
    
    //2D distance
    topRightRotated.Z = 0.0f;
    bottomRightRotated.Z = 0.0f;
    topLeftRotated.Z = 0.0f;


    //chunk ist wie normal ausgerichtet
    float distToTop = FVector::Dist(topRightRotated, playerInLocalSpace);
    float distToBottom = FVector::Dist(bottomRightRotated, playerInLocalSpace);
    topAxisLocked = distToTop > distToBottom; //weiter weg: lock axis
    bottomAxisLocked = !topAxisLocked;
    
    //spieler ist zum chunk seitlich ausgerichtet
    float distToLeft = FVector::Dist(topLeftRotated, playerInLocalSpace);
    float distToRight = FVector::Dist(topRightRotated, playerInLocalSpace);
    leftAxisLocked = distToLeft > distToRight; //weiter weg: lock axis
    rightAxisLocked = !leftAxisLocked;

    /*FString message = FString::Printf(
        TEXT("DEBUGLOCKAXIS top %d, bottom %d, left %d, right %d"),
        topAxisLocked ? 1 : 0,
        bottomAxisLocked ? 1 : 0,
        leftAxisLocked ? 1 : 0,
        rightAxisLocked ? 1 : 0
    );
    */
    //DebugHelper::logMessage(message);
}






bool AcustomWaterActor::playerIsInBounds(){
    FVector location = PlayerInfo::playerLocation();
    return inBoundsOfPane(location);
}

bool AcustomWaterActor::inBoundsOfPane(FVector &vec){
    FVector ownOffset = GetActorLocation();
    FVector bl = BottomLeft + ownOffset;
    FVector tr = TopRight + ownOffset;

    return (vec.X >= bl.X && vec.X < tr.X) && (vec.Y >= bl.Y && vec.Y < tr.Y);
}



