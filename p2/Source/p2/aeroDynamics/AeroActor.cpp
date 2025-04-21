#include "AeroActor.h"
#include "p2/meshgen/ELod.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/gamestart/assetManager.h"
#include <algorithm>
#include "p2/DebugHelper.h"
#include "p2/meshgen/MeshData/aeroDynamic/AeroMeshData.h"
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

}

void AAeroActor::BeginPlay(){
    Super::BeginPlay();
    initMesh();

    linearVelocity = FVector(100, 0, 0);
}

void AAeroActor::initMesh(){
    //materialEnum::wingMaterial
    
    
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



    meshDataMain.calculateNormals();

    //update mesh
    if(Mesh){
        int layer = 0;
        bool enableCollision = true;

        updateMesh(
            *Mesh,
            meshDataMain, //MeshData & otherMesh,
            layer,
            enableCollision
        );

        ApplyMaterial(
            Mesh,
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



void AAeroActor::Tick(float DeltaTime){
    //Super::Tick(deltaTime);

    //debug
    DeltaTime *= 0.01f;

    //todo: überlegen wie man wing mesh aufbaut und testet!

    //return;

    //und an objekt bewegung anhängt!
    //F = mass * acceleration
    //F ist bestimmt aus meshdata
    FVector windVector(-100, 0, 0);
    FVector forceOnMesh = meshDataMain.forceFrom(windVector);

    DebugHelper::logMessage("totalForce", forceOnMesh); //has values
    drawForce(forceOnMesh, DeltaTime); //IS NOT DRAWN PROPERLY

    //pass into F = m * a
    /*
    
    x(t) = x0 + v0t + 1/2*a*t^2
    F = m * a
    a = F / m

    x(t) = x0 + v0*t + 1/2*(gravity + a)*t^2

    */
    float mass = 100.0f; //100kg
    FVector accelerationFromForce = forceOnMesh / mass;

    FVector gravity(0, 0, -980);
    FVector x0 = GetActorLocation();
    FVector v0 = linearVelocity * DeltaTime;
    FVector a = gravity + accelerationFromForce;

    //update v(t) = v0 + at
    linearVelocity += a * DeltaTime;

    //build new x(t)
    FVector xt = x0 + v0 * DeltaTime + 0.5f * a * DeltaTime * DeltaTime;

    //debug
    xt.Z = std::max(xt.Z, 0.0);

    SetActorLocation(xt);

    FVector torque_momentum = meshDataMain.torqueVector(forceOnMesh);
    processTroqueAcceleration(torque_momentum, DeltaTime);
}



void AAeroActor::processTroqueAcceleration(FVector &torque, float DeltaTime){

    FVector Interia(1, 1, 1); //eigentlich soll das eine 3x3 matrix sein die die masse verteilung
    //des körpers beschreibt... das ist jetzt nur sx, sy und sz

    // angualarAcceleration = torque / Interia   //muss ich noch verstehen, mass verteilung
    FVector angualarAcceleration = torque / Interia;
    angularVelocity += angualarAcceleration * DeltaTime; //vt = v0 + at integration

    FVector deltaRotationRad = angularVelocity * DeltaTime;

    //das verstehe ich noch nicht
    FRotator deltaRotator = FRotator::MakeFromEuler(FMath::RadiansToDegrees(deltaRotationRad));
    FRotator currentRotation = GetActorRotation();
    currentRotation = (currentRotation + deltaRotator).GetNormalized();
    SetActorRotation(currentRotation);

}

FVector AAeroActor::transformVektorToLocalSpace(FVector &dir){

    FVector dirRelativeToVelocity = dir - linearVelocity; //AB = B - A

    FRotator r = GetActorRotation();
    FMatrix rotationMatrix = FRotationMatrix(r);
    FMatrix InverseMatrix = rotationMatrix.GetTransposed(); //R^-1 = R^T

    FVector output = InverseMatrix.TransformVector(dirRelativeToVelocity);
    return output;
}





void AAeroActor::drawForce(FVector &force, float deltatime){
    FVector a = GetActorLocation();
    FVector vec = force.GetSafeNormal() * 100.0f;
    FVector b = a + vec;

    DebugHelper::logMessage("debugdrawvec ", vec);

    DebugHelper::showLineBetween(
        GetWorld(),
        a,
        b,
        FColor::Blue,
        1.0f
        //deltatime * 2.0f
    );
}
