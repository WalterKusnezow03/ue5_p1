#include "AeroActor.h"
#include "GameCore/MeshGenBase/ELod.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include <algorithm>
#include "CoreMath/Matrix/MMatrix.h"
#include "DebugPlugin/DebugHelper.h"

#include "p2/_world/worldLevel.h"



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

    MeshTail = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("backtailMesh"));
    MeshTail->SetupAttachment(RootComponent);

}

void AAeroActor::BeginPlay(){
    Super::BeginPlay();
    initMesh();

    linearVelocity = FVector(100, 0, 0);


    FRotator rotation;
    rotation.Yaw = -20.0f;
    //SetActorRotation(rotation);
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

    FVector v5 = v3 + FVector(-70, 0, 0);
    FVector v6 = v2 + FVector(-70, 0, 0);
    FVector v7 = v4 + FVector(-70, 0, 0);
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

    //top
    meshDataMain.appendEfficent(v0, v2, v1);
    meshDataMain.appendEfficent(v0, v3, v2);
    meshDataMain.appendEfficent(v1, v2, v4);

    meshDataMain.appendEfficent(v2, v7, v4);
    meshDataMain.appendEfficent(v2, v6, v7);
    meshDataMain.appendEfficent(v3, v6, v2);
    meshDataMain.appendEfficent(v3, v5, v6);

    //close bottom
    meshDataMain.appendEfficent(v0, v1, v6);
    meshDataMain.appendEfficent(v0, v6, v5);
    meshDataMain.appendEfficent(v1, v7, v6);


    //make wider
    MMatrix wider;
    wider.scale(1.0f, 1.5f, 1.0f);
    meshDataMain.transformAllVertecies(wider);


    //for no reason the whole mesh is flipped.
    AeroMeshData wingLeft = meshDataMain;
    MMatrix flip;
    flip.scale(1.0f, -1.0f, 1.0f);
    wingLeft.transformAllVertecies(flip);
    wingLeft.flipAllTriangles(); //after flip the winding order is wrong!
    meshDataMain.append(wingLeft);

    


    //setup mesh data for up and down wings
    //meshDataUpDownBackWings
    meshDataUpDownBackWings = meshDataMain; //copy backwings but smaller
    MMatrix scaleDown;
    scaleDown.scale(0.5f, 0.5f, 1.0f);
    meshDataUpDownBackWings.transformAllVertecies(scaleDown);

    FVector moveBackAndUp(-500, 0, 100);
    if(MeshBackWings){
        MeshBackWings->SetRelativeLocation(moveBackAndUp);
    }
    if(MeshTail){
        MeshTail->SetRelativeLocation(moveBackAndUp);
    }


    //setup tail mesh
    FVector tailv0(0,0,0);
    FVector tailv1(-100,20,0);
    FVector tailv2(-100,0,100);
    FVector tailv3(-100,0,0);
    meshDataTail.appendEfficent(tailv0, tailv2, tailv1); //aussen dreieck
    meshDataTail.appendEfficent(tailv1, tailv2, tailv3); //hinten dreieck

    AeroMeshData copyTail = meshDataTail;
    copyTail.transformAllVertecies(flip); //flip along y axis
    copyTail.flipAllTriangles(); //after flip the winding order is wrong!
    meshDataTail.append(copyTail);

    



    meshDataMain.calculateNormals();
    meshDataUpDownBackWings.calculateNormals();
    meshDataTail.calculateNormals();

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


    //tail
    if(MeshTail){
        int layer = 0;
        bool enableCollision = true;
        updateMesh(
            *MeshTail,
            meshDataTail, // MeshData & otherMesh,
            layer,
            enableCollision
        );

        ApplyMaterial(
            MeshTail,
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
        ptr = a->Find<materialEnum, UMaterial>(materialEnum::wingMaterial);
        // a->findMaterial(materialEnum::wingMaterial);
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
    output[&meshDataTail] = MeshTail;
    
    return output;
}




void AAeroActor::Tick(float DeltaTime){
    //Super::Tick(deltaTime);

    //debug
    //DeltaTime *= 0.1f;
    
    //debugplayer
    if(false){
        if(AplayerScript *casted = AworldLevel::TGetPlayerReference<AplayerScript>()){
			FVector offsetLocation = GetActorLocation() + FVector(0,0,100);
            casted->SetActorLocation(offsetLocation);
		}
        
    }else{
        DeltaTime *= 0.001f;
    }
    

    //todo: überlegen wie man wing mesh aufbaut und testet!

    //return;

    //und an objekt bewegung anhängt!
    //F = mass * acceleration
    //F ist bestimmt aus meshdata
    int onemeter = 100;
    FVector windVectorWorld(-300 * onemeter, 0, -10); //world space
    //FVector forceOnMesh = meshDataMain.forceFrom(windVector);


    ///----- NEW ------
    FVector torque_momentum(0,0,0);
    FVector forceOnMesh(0,0,0);
    FVector forceOnMeshWorld(0,0,0);


    std::map<AeroMeshData*, UProceduralMeshComponent*> allMeshes = allMeshDataHavingForce();
    for (auto &pair : allMeshes){
        AeroMeshData *currentMesh = pair.first;
        UProceduralMeshComponent *meshComponent = pair.second;
        if (currentMesh && meshComponent)
        {

            FVector windrelative = transformVektorToLocalSpaceAndSpeed(
                meshComponent,
                windVectorWorld // world space
            );
            FVector relativeLocationToAirplanePivot = meshComponent->GetRelativeLocation();


            FVector currentForce(0,0,0);
            FVector currentTorque(0,0,0);
            currentMesh->forceAndTorqueFrom(
                windrelative,
                relativeLocationToAirplanePivot,
                currentForce,
                currentTorque
            );
            
            torque_momentum += currentTorque;

            forceOnMesh += currentForce; //Sum as expected

            //world space (sum)
            forceOnMeshWorld += transformVektorToWorldSpace(meshComponent, currentForce);

        }
    }

    




    //----- OLD ------
    /*
    FVector forceOnMesh(0,0,0);
    FVector forceOnMeshWorld(0,0,0);

    std::map<AeroMeshData*, UProceduralMeshComponent*> allMeshes = allMeshDataHavingForce();
    for (auto &pair : allMeshes){
        AeroMeshData *currentMesh = pair.first;
        UProceduralMeshComponent *meshComponent = pair.second;
        if (currentMesh && meshComponent)
        {

            FVector windrelative = transformVektorToLocalSpaceAndSpeed(
                meshComponent,
                windVectorWorld // world space
            );

            float size = windrelative.Size();
            DebugHelper::showScreenMessage("relative speed: ", size);


            FVector currentForce = currentMesh->forceFrom(windrelative);
            forceOnMesh += currentForce; //Sum as expected

            //world space (sum)
            forceOnMeshWorld += transformVektorToWorldSpace(meshComponent, currentForce);

        }
    }



    //FIND TORQUE / MOMENTUM
    FVector torque_momentum;
    for (auto &pair : allMeshes){
        AeroMeshData *currentMesh = pair.first;
        UProceduralMeshComponent *meshComponent = pair.second;
        if (currentMesh && meshComponent)
        {
            / **
             * CAUTION //might pass actor center of mass!
             * /
            FVector relativeLocationToAirplanePivot = meshComponent->GetRelativeLocation();
            FVector currentTorque = currentMesh->torqueVector(
                forceOnMesh,
                relativeLocationToAirplanePivot
            );
            torque_momentum += currentTorque; //might pass actor center of mass
        }
    }*/

    // DebugHelper::logMessage("totalForce", forceOnMesh); //has values
    drawForce(forceOnMeshWorld, DeltaTime); 

    //forward force
    forceOnMeshWorld += thrustForce();

    //process
    processAeroForceAcceleration(forceOnMeshWorld, DeltaTime);
    
    //erstmal ausblenden
    processTroqueAcceleration(torque_momentum, DeltaTime);
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
    
    float oneKg = 1000.0f; //gramm
    float kg = 1000;
    return oneKg * kg;*/

    return 10000.0f;
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


    float accelerationMs = 400.0f; //4m/s
    FVector accelerationDirMs = forward * accelerationMs;
    FVector FThrustM = MassInKgTotal() * accelerationDirMs; //F = m * a
    FVector FThrustCm = FThrustM * 100.0f;

    //drawForce(FThrustCm, 1.0f); 
    return FThrustCm;
}


FVector AAeroActor::thrustForceLocal(){
    //F = m * a
    FVector forward(1,0,0);
    
    float accelerationMs = 400.0f; //4m/s
    FVector accelerationDirMs = forward * accelerationMs;
    FVector FThrustM = MassInKgTotal() * accelerationDirMs; //F = m * a
    FVector FThrustCm = FThrustM * 100.0f;

    drawForce(FThrustCm, 1.0f); 
    return FThrustCm;
}



void AAeroActor::processTroqueAcceleration(FVector &torque, float DeltaTime){
    float mass = MassInKgTotal();
    FVector torqueMomentumInM = torque / 100.0f;

    // Interia Tensor
    MMatrix interiaInverse;
    interiaInverse.scale(
        1.0f / (mass * 0.1f), 
        1.0f / (mass), 
        1.0f / (mass * 0.1f)
    );
   

    // 1.Berechne Winkelbeschleunigung (α = τ / I  <=> α = I^-1 * τ)
    FVector angularAccelerationInM = interiaInverse * torqueMomentumInM;
    FVector angularAcceleration = angularAccelerationInM * 100.0f;


    // **Dämpfungsmoment hinzufügen**
    if(true){
        const float DampingFactor = 10.0f; // Wert feinjustieren
        FVector dampingTorque = -angularVelocity * DampingFactor;
        FVector totalTorque = torque + dampingTorque;
        totalTorque /= 100.0f;

        angularAccelerationInM = interiaInverse * totalTorque;
        angularAcceleration = angularAccelerationInM * 100.0f;


        //DEBUG
        if(false){
            angularAcceleration.Z = 0.0f;

            angularAcceleration.X *= 0.000001f;
        }


        
    }
    





    //2.Integration der acceleration
    //limitiert durch grösse des vektors.

    //angular velocity = rad / s

    FVector integrateAcceleration = angularAcceleration * DeltaTime;
    angularVelocity += integrateAcceleration;
    if(false){
        if(angularVelocity.Size() >= 0.1f)
            integrateAcceleration *= 1.0f / angularVelocity.Size();
        angularVelocity += integrateAcceleration * 0.00000000001f;
    }
    

    DebugHelper::showScreenMessage("angular velocity ",angularVelocity);

    //3.Integration der velocity
    FRotator x = GetActorRotation();
    FRotator vt = FRotator::MakeFromEuler(FMath::RadiansToDegrees(angularVelocity * DeltaTime));
    FRotator finalRotation = vt + x; //M = 
    SetActorRotation(finalRotation);
    
    
}



/// @brief transforms a direction into actor rotation space
/// @param dirWorldSpace 
/// @return 
FVector AAeroActor::transformVektorToLocalSpaceAndSpeed(FVector &dirWorldSpace){

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
FVector AAeroActor::transformVektorToLocalSpaceAndSpeed(
    UProceduralMeshComponent *component,
    FVector &dirWorldSpace
){
    FVector dirActorSpace = transformVektorToLocalSpaceAndSpeed(dirWorldSpace);
    if(component){
        FRotator rotation = component->GetComponentRotation();
        FMatrix rotationMatrix = FRotationMatrix(rotation);
        FMatrix InverseMatrix = rotationMatrix.GetTransposed(); //R^-1 = R^T

        FVector output = InverseMatrix.TransformVector(dirActorSpace);
        return output;
    }
    return dirActorSpace;
}





FVector AAeroActor::transformVektorToWorldSpace(
    UProceduralMeshComponent *component,
    FVector &dirLocalSpace
){
    FVector dirWorldSpace = transformVektorToWorldSpace(dirLocalSpace);
    if(component){
        FRotator rotation = component->GetComponentRotation();
        FMatrix rotationMatrix = FRotationMatrix(rotation);
        FVector output = rotationMatrix.TransformVector(dirWorldSpace);
        return output;
    }
    return dirWorldSpace;
}

FVector AAeroActor::transformVektorToWorldSpace(FVector &dirLocalSpace){
    FMatrix rotationMatrix = actorRotationMatrix();
    FVector output = rotationMatrix.TransformVector(dirLocalSpace);
    return output;
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
