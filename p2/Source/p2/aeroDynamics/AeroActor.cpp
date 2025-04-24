#include "AeroActor.h"
#include "p2/meshgen/ELod.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/gamestart/assetManager.h"
#include <algorithm>
#include "p2/entities/customIk/MMatrix.h"
#include "p2/DebugHelper.h"
#include "p2/meshgen/MeshData/aeroDynamic/AeroMeshData.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/meshgen/MeshData/MeshData.h"


AAeroActor* AAeroActor::Construct(UWorld *world, FVector &location){
    if(world != nullptr){

        UClass *toSpawn = AAeroActor::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            AAeroActor *spawned = world->SpawnActor<AAeroActor>(
                toSpawn, 
                location, 
                FRotator::ZeroRotator, 
                SpawnParams
            );

            return spawned;
        }
    }
    return nullptr;
}





AAeroActor::AAeroActor() : AcustomMeshActorBase(){

    
    // create back wing sub component!
    MeshBackWings = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("backwingMesh"));
    // Attach it to the RootComponent (Mesh) so it has the same transform
    MeshBackWings->SetupAttachment(RootComponent);

}

void AAeroActor::BeginPlay(){
    Super::BeginPlay();
    initMesh();

    linearVelocity = FVector(100, 0, 0);
}

void AAeroActor::initMesh(){
    //materialEnum::wingMaterials
    
    
    //meshDataMain setup, hauptsache eine schräge fläche ist dabei
    int height = 20;
    FVector v0(0, 0, 0);
    FVector v1(0, 200, 0);
    FVector v2(-100, 200, height);
    FVector v3(-100, 0, height);

    FVector v4(-100, 300, height);

    FVector v5 = v3 + FVector(-30, 0, 0);
    FVector v6 = v2 + FVector(-30, 0, 0);
    FVector v7 = v4 + FVector(-30, 0, 0);
    /*
    1->2
    |  |
    0<-3
    void appendEfficent(
		FVector &a,
		FVector &b,
		FVector &c
	);
    */
    meshDataMain.appendEfficent(v0, v2, v1);
    meshDataMain.appendEfficent(v0, v3, v2);
    meshDataMain.appendEfficent(v1, v2, v4);

    meshDataMain.appendEfficent(v2, v7, v4);
    meshDataMain.appendEfficent(v2, v6, v7);
    meshDataMain.appendEfficent(v3, v6, v2);
    meshDataMain.appendEfficent(v3, v5, v6);

    //make wider
    MMatrix wider;
    wider.scale(1.0f, 2.0f, 1.0f);
    meshDataMain.transformAllVertecies(wider);


    //for no reason the whole mesh is flipped.
    AeroMeshData wingLeft = meshDataMain;
    MMatrix flip;
    flip.scale(1.0f, -1.0f, 1.0f);
    wingLeft.transformAllVertecies(flip);
    wingLeft.flipAllTriangles();
    meshDataMain.append(wingLeft);

    


    //setup mesh data for up and down wings
    //meshDataUpDownBackWings
    meshDataUpDownBackWings = meshDataMain; //copy doesnt work ?



    FVector moveBackAndUp(-100, 0, 50);
    MMatrix scaleDown;
    scaleDown.scale(0.5f, 0.5f, 1.0f);
    meshDataUpDownBackWings.transformAllVertecies(scaleDown);
    if(MeshBackWings){
        MeshBackWings->SetRelativeLocation(moveBackAndUp);
    }
    



    meshDataMain.calculateNormals();
    meshDataUpDownBackWings.calculateNormals();

    //update mesh main (wings)
    if(Mesh){
        int layer = 0;
        bool enableCollision = true;
        updateMesh(
            *Mesh,
            meshDataMain, // MeshData & otherMesh,
            layer,
            enableCollision
        );

        ApplyMaterial(
            Mesh,
            wingMaterialPointer(),
            layer
        );  
    }

    //update mesh back wing
    if(MeshBackWings){
        int layer = 0;
        bool enableCollision = true;
        updateMesh(
            *MeshBackWings,
            meshDataUpDownBackWings, // MeshData & otherMesh,
            layer,
            enableCollision
        );

        ApplyMaterial(
            MeshBackWings,
            wingMaterialPointer(),
            layer
        );  
    }




    //ReloadMeshAndApplyAllMaterials(); // super
}


UMaterialInterface *AAeroActor::wingMaterialPointer(){
    UMaterialInterface *ptr = nullptr;
    if (assetManager *a = assetManager::instance())
    {
        ptr = a->findMaterial(materialEnum::wingMaterial);
    }
    return ptr;
}

/*
std::vector<AeroMeshData *> AAeroActor::allMeshDataHavingForce(){
    std::vector<AeroMeshData *> output;
    output.push_back(&meshDataMain);
    output.push_back(&meshDataUpDownBackWings);

    return output;
}*/

std::map<AeroMeshData*, UProceduralMeshComponent*> AAeroActor::allMeshDataHavingForce(){
    std::map<AeroMeshData*, UProceduralMeshComponent*> output;

    output[&meshDataMain] = Mesh;
    output[&meshDataUpDownBackWings] = MeshBackWings;
    
    return output;
}




void AAeroActor::Tick(float DeltaTime){
    //Super::Tick(deltaTime);

    //debug
    //DeltaTime *= 0.1f;
    
    //debugplayer
    if(true){
        if(referenceManager *r = referenceManager::instance()){
            AplayerScript *player = r->getPlayerPointer();
            if(player){
                player->SetActorLocation(GetActorLocation());
            }
        }
    }
    

    //todo: überlegen wie man wing mesh aufbaut und testet!

    //return;

    //und an objekt bewegung anhängt!
    //F = mass * acceleration
    //F ist bestimmt aus meshdata
    int onemeter = 100;
    FVector windVectorWorld(-1000 * onemeter, 0, -10); //world space
    //FVector forceOnMesh = meshDataMain.forceFrom(windVector);


    FVector forceOnMesh;

    std::map<AeroMeshData*, UProceduralMeshComponent*> allMeshes = allMeshDataHavingForce();
    for (auto &pair : allMeshes){
        AeroMeshData *currentMesh = pair.first;
        UProceduralMeshComponent *meshComponent = pair.second;
        if (currentMesh && meshComponent)
        {

            FVector windrelative = transformVektorToLocalSpace(
                meshComponent,
                windVectorWorld // world space
            );

            FVector currentForce = currentMesh->forceFrom(windrelative);
            forceOnMesh += currentForce; //Sum as expected
        }
    }
    //forward force
    forceOnMesh += thrustForce();

    //FIND TORQUE / MOMENTUM
    FVector torque_momentum;
    for (auto &pair : allMeshes){
        AeroMeshData *currentMesh = pair.first;
        UProceduralMeshComponent *meshComponent = pair.second;
        if (currentMesh && meshComponent)
        {
            /**
             * CAUTION //might pass actor center of mass!
             */
            FVector currentTorque = currentMesh->torqueVector(forceOnMesh);
            torque_momentum += currentTorque; //might pass actor center of mass
        }
    }

    // DebugHelper::logMessage("totalForce", forceOnMesh); //has values
    drawForce(forceOnMesh, DeltaTime); 

    //process
    processAeroForceAcceleration(forceOnMesh, DeltaTime);
    
    //erstmal ausblenden
    //processTroqueAcceleration(torque_momentum, DeltaTime);
}


void AAeroActor::processAeroForceAcceleration(FVector &forceOnMesh, float DeltaTime){
    //pass into F = m * a
    /*
    
    x(t) = x0 + v0t + 1/2*a*t^2
    F = m * a
    a = F / m

    x(t) = x0 + v0*t + 1/2*(gravity + a)*t^2

    */


    //test
    FVector forceInMeter = forceOnMesh / 100.0f;

    float mass = MassInKgTotal();
    FVector accelerationFromForceInMeter = forceInMeter / mass;
    FVector accelerationFromForce = accelerationFromForceInMeter * 100.0f;


    FVector gravity(0, 0, -980);
    FVector x0 = GetActorLocation();
    if(x0.Z <= 0.0f){
        return;
    }

    FVector v0 = linearVelocity * DeltaTime;
    FVector a = gravity + accelerationFromForce;

    //update v(t) = v0 + at
    linearVelocity += a * DeltaTime;

    //build new x(t) = x0 + v0*t + 1/2*a*t^2
    FVector xt = x0 + v0 * DeltaTime + 0.5f * a * DeltaTime * DeltaTime;

    //debug
    xt.Z = std::max(xt.Z, 0.0);

    SetActorLocation(xt);
}


float AAeroActor::MassInKgTotal(){
    
    /*
    F = m * a    mit F in m = m in kg * a in m
    F in cm = m in gramm * (cm/100)
    */
    float oneKg = 1000.0f; //gramm
    float kg = 500;
    return oneKg * kg;
}

/**
 * TODO: Forward acceleration to remove drag! 
 */

/// @brief Thrust (forward) force in CM 
FVector AAeroActor::thrustForce(){
    //F = m * a
    FVector forward(1,0,0);
    FMatrix rot = actorRotationMatrix();
    forward = rot.TransformVector(forward);


    float accelerationMs = 400.0f;
    FVector accelerationDirMs = forward * accelerationMs;
    FVector FThrustM = MassInKgTotal() * accelerationDirMs; //F = m * a
    FVector FThrustCm = FThrustM * 100.0f;

    drawForce(FThrustCm, 1.0f); 
    return FThrustCm;
}



void AAeroActor::processTroqueAcceleration(FVector &torque, float DeltaTime){
    float mass = MassInKgTotal();
    FVector torqueMomentumInM = torque / 100.0f;

    // Diagonale des Inertia-Tensors -- soll noch 3x3 matrix sein!
    FVector inertia(mass, mass, mass); 

    // 1. Berechne Winkelbeschleunigung (α = τ / I)
    FVector angularAccelerationInM = torqueMomentumInM / inertia;
    FVector angularAcceleration = angularAccelerationInM * 100.0f;

    // 2. Integriere Winkelgeschwindigkeit
    angularVelocity += angularAcceleration * DeltaTime;

    // 3. Dämpfung (sonst dreht sich alles für immer)
    float angularDamping = 0.9f;
    angularVelocity *= angularDamping;

    FVector deltaRotationRad = angularVelocity * DeltaTime;

    //das verstehe ich noch nicht
    FRotator deltaRotator = FRotator::MakeFromEuler(FMath::RadiansToDegrees(deltaRotationRad));
    FRotator currentRotation = GetActorRotation();
    currentRotation = (currentRotation + deltaRotator).GetNormalized();
    SetActorRotation(currentRotation);

}

/// @brief transforms a direction into actor rotation space
/// @param dirWorldSpace 
/// @return 
FVector AAeroActor::transformVektorToLocalSpace(FVector &dirWorldSpace){

    FVector dirRelativeToVelocity = dirWorldSpace - linearVelocity; //AB = B - A

   
    FMatrix rotationMatrix = actorRotationMatrix();
    FMatrix InverseMatrix = rotationMatrix.GetTransposed(); //R^-1 = R^T

    FVector output = InverseMatrix.TransformVector(dirRelativeToVelocity);
    return output;
}

FMatrix AAeroActor::actorRotationMatrix(){
    FRotator r = GetActorRotation();
    FMatrix rotationMatrix = FRotationMatrix(r);
    return rotationMatrix;
}


//testing needed
/// @brief transforms a direction into actor rotation space
/// @param component component relative, own rotation if needed 
/// @param dirWorldSpace world space vector
/// @return transformed vecto in actor AND relative mesh rotation space
FVector AAeroActor::transformVektorToLocalSpace(
    UProceduralMeshComponent *component,
    FVector &dirWorldSpace
){
    FVector dirActorSpace = transformVektorToLocalSpace(dirWorldSpace);
    if(component){
        FRotator rotation = component->GetComponentRotation();
        FMatrix rotationMatrix = FRotationMatrix(rotation);
        FMatrix InverseMatrix = rotationMatrix.GetTransposed(); //R^-1 = R^T

        FVector output = InverseMatrix.TransformVector(dirActorSpace);
        return output;
    }
    return dirActorSpace;
}



void AAeroActor::drawForce(FVector &force, float deltatime){
    FVector a = GetActorLocation();
    FVector vec = force.GetSafeNormal() * 100.0f;
    FVector b = a + vec;

    //DebugHelper::logMessage("debugdrawvec ", vec);

    DebugHelper::showLineBetween(
        GetWorld(),
        a,
        b,
        FColor::Blue,
        1.0f
        //deltatime * 2.0f
    );
}
