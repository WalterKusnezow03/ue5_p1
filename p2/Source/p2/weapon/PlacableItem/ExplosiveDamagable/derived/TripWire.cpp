#include "TripWire.h"
#include "p2/entityManager/AlertManager.h"


ATripWire::ATripWire() : Super(){
    rootScene = CreateDefaultSubobject<USceneComponent>(TEXT("USceneRoot"));
    RootComponent = rootScene;

    // Create the ProceduralMeshComponent
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    Mesh->SetupAttachment(RootComponent);
}

void ATripWire::BeginPlay(){
    Super::BeginPlay();
    SetupWire();
}


void ATripWire::Tick(float deltatime){
    Super::Tick(deltatime);

    TickUpdateBound();
}

void ATripWire::TickUpdateBound(){

    FVector ownLocation = GetActorLocation();
    TArray<FVector> positions;

    AlertManager::EntitiesInRadius(
		ownLocation,
		TriggerDistance,
		positions
	);
    if(AnyIntersectWithWire(positions)){
        Detonate();
    }
}

bool ATripWire::AnyIntersectWithWire(TArray<FVector> &positions){
    for (int i = 0; i < positions.Num(); i++){
        if(AnyIntersectWithWire(positions[i])){
            return true;
        }
    }
    return false;
}

bool ATripWire::AnyIntersectWithWire(FVector &position){
    //TO BE IMPLEMENTED
    




    return false;
}



void ATripWire::SetupWire(){
    //wireMesh
    if(Mesh){
        //override pcm component
        wireMesh.overrideMeshPair(Mesh, nullptr);
    
        //set up vertex data
        TArray<FVector> vertecies;
        generateVertexBuffer(vertecies);
        if(vertecies.Num() == 4){
            FVector dir(0, 0, 10);
            MeshData &meshdata = TripWireMeshData();
            meshdata.appendCube(
                vertecies[0],
                vertecies[1],
                vertecies[2],
                vertecies[3],
                dir // Up
            );
        }
        wireMesh.updateMeshAll(); //Update procedural mesh component

    
    }
}

void ATripWire::generateVertexBuffer(TArray<FVector> &vertecies){
    //x is forward
    //y is right side
    /*
    lower side
    1 ->2
    |   |
    0 <-3
    */
    
    vertecies.Add(FVector(0, -5, 0));
    vertecies.Add(FVector(100, -5, 0));
    vertecies.Add(FVector(100, 5, 0));
    vertecies.Add(FVector(0, 5, 0));
}

void ATripWire::UpdateWireLocations(){
    if(Mesh){
        MeshData &meshdata = TripWireMeshData();
        TArray<FVector> &rawMeshData = meshdata.getVerteciesRef();
        UpdateWireLocations(rawMeshData);

        wireMesh.refreshMeshRaycast(meshMaterial());
    }
}

void ATripWire::UpdateWireLocations(TArray<FVector> &rawMeshData){
    //erste 4 sind lower, die anderen upper.
    //erste 2 lower und upper am actor, rest extended
    if(rawMeshData.Num() == 8){
        FVector &v0 = rawMeshData[0];
        FVector &v1 = rawMeshData[1];
        FVector &v2 = rawMeshData[2];
        FVector &v3 = rawMeshData[3];
        UpdateWireLocationStartingQuad(v0, v1, v2, v3);

        FVector &v4 = rawMeshData[4];
        FVector &v5 = rawMeshData[5];
        FVector &v6 = rawMeshData[6];
        FVector &v7 = rawMeshData[7];
        UpdateWireLocationEndingQuad(v4, v5, v6, v7);
    }
}

void ATripWire::UpdateWireLocationStartingQuad(
    FVector &v0,
    FVector &v1,
    FVector &v2,
    FVector &v3
){
    float halfWidth = std::abs(widthWire / 2.0f);
    v0 = FVector(0, -halfWidth, -halfWidth);
    v1 = FVector(0, -halfWidth, halfWidth);
    v2 = FVector(0, halfWidth, halfWidth);
    v3 = FVector(0, halfWidth, -halfWidth);
}

void ATripWire::UpdateWireLocationEndingQuad(
    FVector &v0,
    FVector &v1,
    FVector &v2,
    FVector &v3
){
    //make clean quad
    UpdateWireLocationStartingQuad(v0, v1, v2, v3);

    //move quad
    FVector endingLocationCenter;
    if(RaycastForwardLocal(endingLocationCenter)){
        v0 += endingLocationCenter;
        v1 += endingLocationCenter;
        v2 += endingLocationCenter;
        v3 += endingLocationCenter;
        return;
    }

    if(RaycastDownLocal(endingLocationCenter)){
        v0 += endingLocationCenter;
        v1 += endingLocationCenter;
        v2 += endingLocationCenter;
        v3 += endingLocationCenter;
        return;
    }
}



MeshData &ATripWire::TripWireMeshData(){
    return wireMesh.meshDataReferenceRaycast(meshMaterial());
}

materialEnum ATripWire::meshMaterial(){
    return materialEnum::stoneMaterial;
}


bool ATripWire::RaycastForwardLocal(FVector &outposition){
    FVector start = GetActorLocation();
    FVector forward(100, 0, 0);
    
    //rotate
    FTransform TR = GetActorTransform();
    FVector end = TR.TransformPosition(forward); //transformed position is end.

    return PerformRaycastLocalHit(start, end, outposition);
}

bool ATripWire::RaycastDownLocal(FVector &outposition){
    FVector start(100, 0, 0);

    //transform to world space
    FTransform TR = GetActorTransform();
    start = TR.TransformPosition(start);

    FVector down = start + FVector(0, 0, -100);
    return PerformRaycastLocalHit(start, down, outposition);
}

bool ATripWire::PerformRaycastLocalHit(FVector &start, FVector &end, FVector &outposition){
    if(PerformRaycast(start, end, outposition)){
        //transform to local space
        FTransform TR = GetActorTransform();
        outposition = TR.InverseTransformPosition(outposition);
        return true;
    }
    return false;
}

bool ATripWire::PerformRaycast(FVector &start, FVector &end, FVector &outposition){

    // Perform the raycast
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore the actor itsels
    Params.bTraceComplex = false; //new lower complexity

    

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params);


    // If the raycast hit something, save hitresult and return positive
    if (bHit)
    {
        outposition = HitResult.ImpactPoint;
        return true;
    }
    return false;
}
