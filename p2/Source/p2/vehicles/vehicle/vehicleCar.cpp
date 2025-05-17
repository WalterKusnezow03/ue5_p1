#include "vehicleCar.h"
#include "p2/entities/customIk/MMatrix.h"


AvehicleCar *AvehicleCar::Construct(UWorld *world){
    if(world){
        UClass *toSpawn = AvehicleCar::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            FVector location(0, 0, 25);
            AvehicleCar *spawned = world->SpawnActor<AvehicleCar>(
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

AvehicleCar::AvehicleCar() : AvehicleBase(){


}


void AvehicleCar::BeginPlay(){
    Super::BeginPlay();
    createWheels();
    createBaseCube();
}

void AvehicleCar::createWheels(){
    UWorld *world = GetWorld();
    float radius = 25.0f;

    //x is forward axis !
    frontRight = Awheel::Construct(world, radius, this, FVector(200,-50,0));
    frontLeft = Awheel::Construct(world, radius, this, FVector(200,50,0));
    rearRight = Awheel::Construct(world, radius, this, FVector(-200, -50,0));
    rearLeft = Awheel::Construct(world, radius, this, FVector(-200, 50, 0)); 
}

void AvehicleCar::createBaseCube(){
    MeshData &ref = findMeshDataReference(
        materialEnum::wallMaterial,
        true // has raycast
    );

    /*
    1->2
    |  |
    0<-3
    */
    FVector b(200,-50,0);
    FVector c(200,50,0);
    FVector a(-200, -50,0);
    FVector d(-200, 50, 0);
    FVector dir(0, 0, 20);
    ref.appendCube(a, b, c, d, dir);

    ReloadMeshAndApplyAllMaterials();
}

void AvehicleCar::Tick(float deltaTime){
    Super::Tick(deltaTime);


}

void AvehicleCar::pushGasPedal(float scalar){
    //clamp
    scalar = std::min(scalar, 1.0f);
    scalar = std::max(scalar, 0.0f);

    gasPedalScalar = scalar;
}

float AvehicleCar::radPerSecondAcceleration(){
    float brake = 0.0f;
    float accelerationOfMotor = MMatrix::degToRadian(100.0f);
    return gasPedalScalar * brake * accelerationOfMotor;
}

void AvehicleCar::TickWheels(float deltaTime){
    FVector deltaDirection(0, 0, 0);

    float radPerSecond = radPerSecondAcceleration();
    if(rearRight != nullptr){
        deltaDirection += rearRight->TickAngularAccelerationAndGetVelocity(radPerSecond, deltaTime);
    } 
    if(rearLeft != nullptr){
        deltaDirection += rearLeft->TickAngularAccelerationAndGetVelocity(radPerSecond, deltaTime);
    }
    
    

    deltaDirection /= 2.0f; //strecke zurück gelegt

    //apply velocity (?)
    
    //braucht man für slip ratio oder, drift. Keine ahnung man.
    deltaDirection = Super::moveDirectionToLocalRotationSpace(deltaDirection); //super::

    applyDeltaRotation();
    applyForces(deltaTime);
}

void AvehicleCar::applyForces(float deltaTime){

    //alle kräfte aussummieren aus reifen

    //f total = sum(wheel forces)
    //F = m * a
    //a = F / M

    //integrieren
    TArray<Awheel *> allWheels = {
        frontRight, frontLeft, rearRight, rearLeft
    };


    FVector normal(0,0,1); //aus reifen ebene
    float slipAngle = SlipAngle();
    FVector totalForce(0, 0, 0);
    for (int i = 0; i < allWheels.Num(); i++)
    {
        Awheel *current = allWheels[i];
        if(current){

            totalForce += current->AllForces(normal, slipAngle);
        }
    }

    float mass = 2000.0f;

    FVector forceInMeter = totalForce / 100.0f;
    FVector accelerationMS2 = forceInMeter / mass;
    FVector acceleration = accelerationMS2 * 100.0f;

    IntegrateMovement(acceleration, deltaTime); //nach kraft umgestellt.
}

void AvehicleCar::IntegrateMovement(FVector &acceleration, float deltaTime){
    //to world
    FVector accelerationWorld = moveDirectionToWorldRotationSpace(acceleration);
    
    //add gravity
    FVector gravity(0, 0, -981);
    accelerationWorld += gravity * deltaTime;

    
    //v(t) = v0 + at
    velocityLocal += acceleration * deltaTime;
    velocityWorld += accelerationWorld * deltaTime;

    //x(t) = x0 + v0t + 1/2at^2
    FVector x0 = translation.getTranslation();
    FVector velocity = velocityWorld;
    FVector accelerationPart = 0.5f * accelerationWorld * accelerationWorld;

    FVector xt = x0 + velocity + accelerationPart;
    translation.setTranslation(xt);
    SetActorLocation(xt);
}

//achtung stimmt ggf nicht
void AvehicleCar::applyDeltaRotation(){
    float rearFrontDistance = 5.0f;

    if (frontRight != nullptr){
        float theta = frontRight->thetaYawRad();

        //ankathete = rotationRadius,
        //theta dreieck gegeben durch steer angle weil orthogonales dreieck
        //geg khatete ist abstand front to rear

        float tanTheta = std::tan(theta);
        if(isNotZero(tanTheta)){
            float rotationRadius = rearFrontDistance / tanTheta;
            if(isNotZero(rotationRadius)){
                float angularSpeed = velocity() / rotationRadius;
            }
        }
    }
}




bool AvehicleCar::isNotZero(float value){
    return std::abs(value) > 0.001f;
}
