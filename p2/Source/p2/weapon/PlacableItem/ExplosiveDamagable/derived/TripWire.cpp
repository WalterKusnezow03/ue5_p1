#include "TripWire.h"
#include "p2/entityManager/AlertManager.h"
#include "DebugPlugin/DebugHelper.h"


void ATripWire::BeginPlay(){
    Super::BeginPlay();
    FindPCMComponent();
    FindWireStartingComponent();
    SetupWire();
}


void ATripWire::FindPCMComponent(){
    TTryAssignByNameContains(PCMSceneComponentName, Mesh);
}


void ATripWire::FindWireStartingComponent(){
    TTryAssignByNameContains(wireStartSceneComponentName, wireStart);
}



void ATripWire::Tick(float deltatime){
    Super::Tick(deltatime);

    TickUpdateBound();
}

void ATripWire::TickUpdateBound(){
    if(isDetonated){
        return;
    }

    FVector ownLocation = GetActorLocation();
    TArray<FVector> positions;

    AlertManager::EntitiesInRadiusFootPositions(
		ownLocation,
		TriggerDistance,
		positions
	);
    if(AnyIntersectWithWireWorld(positions)){
        Detonate();
    }
}

bool ATripWire::AnyIntersectWithWireWorld(TArray<FVector> &positions){
    for (int i = 0; i < positions.Num(); i++){
        if(AnyIntersectWithWireWorld(positions[i])){
            return true;
        }
    }
    return false;
}

bool ATripWire::AnyIntersectWithWireWorld(FVector &position){
    FVector Origin;
    FVector BoxExtent;

    GetActorBounds(
        false,      //bOnlyCollidingComponents
        Origin,     //center of the bounds
        BoxExtent   //half-size of the bounds
    );
    BoxExtent *= 1.5f;

    DebugHelper::showBox(GetWorld(), Origin, BoxExtent, FColor::Red, 0.1f);

    FBox Box = FBox::BuildAABB(Origin, BoxExtent);
    return Box.IsInside(position); //world
}


bool ATripWire::AnyIntersectWithWire(FVector &position){
    //TO BE IMPLEMENTED
    MeshData &meshdata = TripWireMeshData();
    if(meshdata.isInsideBoundingbox(position)){
        return true;
    }
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
    FVector start = WireStartLocation();

    vertecies.Add(start + FVector(0, -5, 0)); //start
    vertecies.Add(start + FVector(100, -5, 0)); //end
    vertecies.Add(start + FVector(100, 5, 0)); //end
    vertecies.Add(start + FVector(0, 5, 0)); //start
}

FVector ATripWire::WireStartLocation(){
    FVector start(0, 0, 0);
    if (wireStart)
    {
        start = wireStart->GetRelativeLocation();
    }
    return start;
}

void ATripWire::UpdateWireLocations(){
    if(Mesh){
        MeshData &meshdata = TripWireMeshData();
        TArray<FVector> &rawMeshData = meshdata.getVerteciesRef();
        UpdateWireLocations(rawMeshData);

        wireMesh.refreshMeshRaycast(meshMaterial());
    }
}

void ATripWire::OnPickup(){
    Super::OnPickup();
    ShowWire(false);
}

void ATripWire::ShowWire(bool flag){
    if(Mesh){
        Mesh->SetHiddenInGame(!flag);
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
    FVector wireStartCopy = WireStartLocation();
    float halfWidth = std::abs(widthWire / 2.0f);
    v0 = FVector(0, -halfWidth, -halfWidth) + wireStartCopy;
    v1 = FVector(0, -halfWidth, halfWidth) + wireStartCopy;
    v2 = FVector(0, halfWidth, halfWidth) + wireStartCopy;
    v3 = FVector(0, halfWidth, -halfWidth) + wireStartCopy;
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
    if(RaycastForwardAndDownLocal(endingLocationCenter)){
        v0 += endingLocationCenter;
        v1 += endingLocationCenter;
        v2 += endingLocationCenter;
        v3 += endingLocationCenter;
        return;
    }


    if(RaycastForwardLocal(endingLocationCenter)){
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


bool ATripWire::RaycastForwardAndDownLocal(FVector &outposition){
    
    //update needed here
    FVector forwardUp(lengthWire, 0, 50);
    FVector forwardDown(lengthWire, 0, -50);

    //rotate / transform into world space
    FTransform TR = GetActorTransform();
    FVector start = TR.TransformPosition(forwardUp);
    FVector end = TR.TransformPosition(forwardDown); //transformed position is end.

    return PerformRaycastLocalHit(start, end, outposition);
}

bool ATripWire::RaycastForwardLocal(FVector &outposition){
    FVector start = GetActorLocation();
    FVector forward(lengthWire, 0, 0);
    
    //rotate
    FTransform TR = GetActorTransform();
    FVector end = TR.TransformPosition(forward); //transformed position is end.

    return PerformRaycastLocalHit(start, end, outposition);
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



USceneComponent *ATripWire::FindHandCarriedScene(EArmType type){
    if(type == EArmType::ELeft){
        return nullptr;
    }
    if(type == EArmType::ERight){
        return rootScene;
    }
    return nullptr;
}


#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
void ATripWire::SpawnItemAtLocation(FVector &location, FVector &normal){
    //spawn c4 here at location
    //add to children!
    if(EntityManager *e = AworldLevel::entityManager()){
        Aweapon *weapon = e->spawnAweapon(GetWorld(), weaponEnum::tripWire);
        if(weapon != nullptr){
            if(ATripWire *casted = Cast<ATripWire>(weapon)){
                casted->Type = weaponEnum::tripWire;
                casted->ResetFlagsAndProperties();
                casted->drop(); // remove from any owner.
                
                casted->setTeam(teamEnum::neutralTeam);    
                casted->showWeapon(true);

                casted->SetLocationAndLookDir(location, normal);

                casted->ShowWire(true);
                casted->UpdateWireLocations();
            }
        }   
    }
}