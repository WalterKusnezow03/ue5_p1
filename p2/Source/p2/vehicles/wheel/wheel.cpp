#include "wheel.h"
#include "p2/meshgen/foliage/helper/FVectorShape.h"


/// @brief creates an instance of an wheel
/// @param world 
/// @param radiusIn 
/// @return 
Awheel *Awheel::Construct(UWorld *world, float radiusIn){
    if(world != nullptr){

        UClass *toSpawn = Awheel::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            FVector location(0, 0, 0);
            Awheel *spawned = world->SpawnActor<Awheel>(
                toSpawn,
                location,
                FRotator::ZeroRotator,
                SpawnParams
            );
            if(spawned != nullptr){
                spawned->generateMesh(radiusIn);
            }

            return spawned;
        }
    }

    return nullptr;
}

Awheel::Awheel() : AcustomMeshActorBase(){

}

void Awheel::generateMesh(float radiusIn){
    radius = std::abs(radiusIn);



    MeshData &meshDataForTire = findMeshDataReference(
        materialEnum::stoneMaterial,
        true // has raycast
    );



    FVectorShape shape;
    int detail = 360;
    shape.createCircleShape(radius, detail); // is around yaw, need to pitch 90
    MMatrix pitchUp;
    pitchUp.pitchRadAdd(MMatrix::degToRadian(90.0f));
    pitchUp.yawRadAdd(MMatrix::degToRadian(90.0f)); //sideways

    shape.moveVerteciesWith(pitchUp);

    bool sideFlag = true;

    std::vector<FVector> line0 = shape.vectorCopy();
    MeshData leftSideMeshData = shape.closeMeshAtCenter(!sideFlag);
    meshDataForTire.append(leftSideMeshData);

    //create right side
    FVector offsetDir(0, 30, 0);
    MMatrix offset(offsetDir);
    shape.moveVerteciesWith(offset);
    std::vector<FVector> line1 = shape.vectorCopy();

    MeshData rightSideMeshData = shape.closeMeshAtCenter(sideFlag);
    meshDataForTire.append(rightSideMeshData);

    //merge paralell quads / append paralell lines
    for (int i = 1; i < line0.size(); i++){
        if(i < line1.size()){
            /*
            1->2
            |  |
            0<-3
            */
            FVector &v0 = line0[i-1];
            FVector &v1 = line0[i];
            FVector &v2 = line1[i];
            FVector &v3 = line1[i - 1];

            meshDataForTire.appendEfficent(v0, v1, v2, v3);
        }
    }
    meshDataForTire.calculateNormals();

    ReloadMeshAndApplyAllMaterials();
}


/// @brief ccreates a wheel actor and attaches it to the given parent
/// @param world 
/// @param radiusIn 
/// @param parent 
/// @param relativeLocation 
/// @return 
Awheel *Awheel::Construct(
    UWorld *world,
    float radiusIn,
    AActor *parent,
    FVector relativeLocation
){
    Awheel *ptr = Construct(world, radiusIn);
    if(ptr != nullptr){
        ptr->AttachToParent(parent, relativeLocation);
    }
    return ptr;
}



/// @brief attaches the wheel to the parent actor (e.x. chasis of car)
/// @param actor 
/// @param relativeLocation 
void Awheel::AttachToParent(AActor *ParentActor, FVector &relativeLocation){
    if(ParentActor != nullptr){
        this->AttachToActor(ParentActor, FAttachmentTransformRules::KeepRelativeTransform);
        
        // Set the relative location of the child relative to the parent
        if (USceneComponent* RootComp = GetRootComponent()){
            RootComp->SetRelativeLocation(relativeLocation); //Offset from parent
        }
    }
}

void Awheel::BeginPlay(){
    Super::BeginPlay();
}


void Awheel::Tick(float deltatime){
    Super::Tick(deltatime);
}


float Awheel::thetaYawRad(){
    return yawAngle;
}

/// @brief returns the distance traveled
/// @param radPerSecond 
/// @param deltaTime 
/// @return 
FVector Awheel::TickAngularAccelerationAndGetVelocity(float radPerSecond, float deltaTime){
    //angularVelocity = w0 + a * t
    angularVelocity += radPerSecond * deltaTime;

    //s zurück gelegt 
    //s = angularVelocity * Radius 
    FVector deltaDirection(
        angularVelocity * radius * deltaTime, //x is forward!
        0,
        0
    );
    deltaDirection = moveToWorldRotationSpace(deltaDirection); //bei fwd ist das notwendig
    return deltaDirection;
}


/// apply rotation after vehicle has moved ?
void Awheel::processDirectionFromVehicleSpace(FVector &deltaDirection){
    //direction sollte nur x und z value haben

    //achtung in wheel space bringen!
    FVector deltaDirectionLocal = moveToLocalRotationSpace(deltaDirection);

    /*
    gegeben s delta

    gesucht angularVelocity after movement


    angularVelocity = w0 + a * t
    s = angularVelocity * radius * deltaTime

    gegeben s:
    s = angularVelocity * radius
    s / radius = angularVelocity
    
    */ 
    float newAngularVelocity = deltaDirectionLocal.Size() / radius; //.size falls die ebene schief ist ?
    //oder doch nur x und z ? 
    angularVelocity = newAngularVelocity;
}






FVector Awheel::moveToLocalRotationSpace(FVector &dir){
    MMatrix rotationInverse = rotation;
    rotationInverse.transpose();
    return rotationInverse * dir;
}

FVector Awheel::moveToWorldRotationSpace(FVector &localDir){
    FVector result = rotation * localDir;
    return result;
}


FVector Awheel::forwardDir(){
    FVector forwardDefault(1, 0, 0);
    FVector result = rotation * forwardDefault;
    return result.GetSafeNormal();
}

FVector Awheel::AllForces(FVector &normal, float slipAngle){
    FVector sum =
        tractionForce() +
        corneringForce(slipAngle) +
        frictionForceRoadDry(normal);

    return sum;
}

/*
ist nicht getestet und muss ausserdem noch verstanden werden
*/
FVector Awheel::tractionForce(){
    //f traction = Torque / radiusWheel angeblich

    //Winkelbeschleunigung (α = τ / I  <=> α = I^-1 * τ)
    //alpha = t / interia 
    //t = alpha * interia
    
    FVector interia(1.0f, 1.0f, 1.0f); //entspricht diagonal matrix
    FVector torqueIntegriert = angularVelocity * interia; //darf man das so ?

    FVector force = torqueIntegriert / radius;
    return force;
}


FVector Awheel::corneringForce(float slipAngle){
    /*
    
    In high speed cornering, the tires develop lateral forces also known as the 
    cornering force. This force depends on the slip angle (alpha), which is the 
    angle between the tire’s heading and its direction of travel, and the weight on 
    that tire
    
    achtung: reifen ist im lokalen koordinaten system,
    heisst, slip angle = Dot(xAxis, tireForward)
    */
    


    //is lateral force, rothogonal zu reifen richtung und up
    FVector forward = forwardDir();

    FVector lateralForceDirection = FVector::CrossProduct(forward, FVector(0, 0, 1)); // orthogonal zu up und forward
    lateralForceDirection = lateralForceDirection.GetSafeNormal();


    //cornering stiffness [in N/rad] (z. B. 5000–150000) was ist das ?
    float stiffness = 5000.0f;
    FVector corneringForce = -1.0f * stiffness * slipAngle * lateralForceDirection;

    return FVector(0, 0, 0);
}




FVector Awheel::frictionForceRoadDry(FVector &normal){
    float haftreibungszahlStrasseTrocken = 0.6f; //Gummi auf Beton
    //trocken 0,6 – 0,9
    //nass 0,4 – 0,6
    return frictionForce(normal, haftreibungszahlStrasseTrocken);
}

FVector Awheel::frictionForceRoadWet(FVector &normal){
    float haftreibungszahlStrasseNass = 0.4f;
    return frictionForce(normal, haftreibungszahlStrasseNass);
}

FVector Awheel::frictionForce(FVector &normal, float haftReibungszahl){ //must be normalized
    FVector fgravity(0, 0, -981);


    float dotproduct = FVector::DotProduct(normal, fgravity * -1.0f);
    FVector fNormal = normal * dotproduct;

    FVector fritctionForce = haftReibungszahl * fNormal;
    return fritctionForce;
}
