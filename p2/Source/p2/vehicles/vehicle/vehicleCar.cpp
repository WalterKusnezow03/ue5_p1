#include "vehicleCar.h"
#include "GameCore/Input/InputContainer.h"
#include "CoreMath/Matrix/MMatrix.h"
#include <algorithm>


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
    PrimaryActorTick.bCanEverTick = true;
}


void AvehicleCar::BeginPlay(){
    Super::BeginPlay(); //needed for transform setup
    
    findWheels();
    findRearFrontDistance();
}



void AvehicleCar::findRearFrontDistance(){
    rearFrontDistance = 500.0f;
    if(frontRight != nullptr && rearRight != nullptr){
        FVector a = frontRight->GetActorLocation();
        FVector b = rearRight->GetActorLocation();
        rearFrontDistance = FVector::Dist(a, b);
    }
}


void AvehicleCar::findWheels(){
    
    TArray<UChildActorComponent*> ChildActorComponents;
    GetComponents(ChildActorComponents);
    for (UChildActorComponent* Comp : ChildActorComponents)
    {
        if (Comp && Comp->GetChildActor())
        {
            AActor *ChildActor = Comp->GetChildActor();
            if(ChildActor){
                FString name = Comp->GetName();
                DebugHelper::logMessage("debugvehicle child name ", name);
                Awheel *wheelCasted = Cast<Awheel>(ChildActor);
                if(wheelCasted != nullptr){
                    if(name.Contains("FrontRight")){
                        frontRight = wheelCasted;
                        frontRight->disableTraction();
                    }
                    if(name.Contains("FrontLeft")){
                        frontLeft = wheelCasted;
                        frontLeft->disableTraction();
                    }
                    if(name.Contains("RearRight")){
                        rearRight = wheelCasted;
                    }
                    if(name.Contains("RearLeft")){
                        rearLeft = wheelCasted;
                    }
                }
            }
        }
    }
}






void AvehicleCar::Tick(float deltaTime){
    Super::Tick(deltaTime);
    
    processDriverInput(deltaTime);
    TickWheels(deltaTime);
}

/// @brief processes the driver input container and applys gas and steering to the car
void AvehicleCar::processDriverInput(float deltatime){
    if(driverInterface != nullptr){

        //set location
        FVector seatLocation = GetActorLocation(); //erstmal so
        driverInterface->setDriverLocation(seatLocation);

        //input
        InputContainer &inputRef = driverInterface->input();
        pushGasPedal(inputRef.forwardAxisValue());
        steer(deltatime, inputRef.rightAxisValue());

    }else{
        pushGasPedal(0.0f);
    }
}


void AvehicleCar::steer(float deltatime, float scalarDirection){
    float steer = steerSpeedRadPerSecond * deltatime * scalarDirection;
    if(frontRight != nullptr && frontLeft != nullptr){
        frontRight->addYaw(steer);
        frontLeft->copyRotation(frontRight);
    }
}

void AvehicleCar::pushGasPedal(float scalar){
    //clamp
    scalar = std::min(scalar, 1.0f);
    scalar = std::max(scalar, 0.0f);

    gasPedalScalar = scalar;

    //ok
    if(showScreenLog && false) DebugHelper::showScreenMessage("gas pedal ", (float) scalar);
}

/// @brief reduces acceleration if brake > 0.0f, otherwise 1.0f
/// @return 
float AvehicleCar::brakeScalar(){
    float brake = 0.0f;
    float invert = 1.0f - brake;
    return invert;
}

float AvehicleCar::radPerSecondAcceleration(){
    float accelerationOfMotor = 500.0f * 100.0f;
    return gasPedalScalar * brakeScalar() * accelerationOfMotor;
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
    

    if(frontLeft != nullptr){
        frontLeft->processVelocityFromVehicleSpace(velocityLocal);
    }
    if(frontRight != nullptr){
        frontRight->processVelocityFromVehicleSpace(velocityLocal);
    }

    


    applyDeltaRotation(deltaTime);
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
    float massPerWheel = mass / allWheels.Num();
    for (int i = 0; i < allWheels.Num(); i++)
    {
        Awheel *current = allWheels[i];
        if(current){
            totalForce += current->AllForces(
                normal, 
                slipAngle, 
                massPerWheel,
                velocityLocal
            );
        }
    }

    if(showScreenLog){
        DebugHelper::showScreenMessage("Slip: ", (float)slipAngle);
        DebugHelper::showScreenMessage("force ", totalForce);
    }
    

    

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
    accelerationWorld += gravity;
    acceleration += gravity;

    
    //v(t) = v0 + at
    velocityLocal += acceleration * deltaTime;
    velocityWorld += accelerationWorld * deltaTime;

    //x(t) = x0 + v0t + 1/2*a*t^2
    FVector x0 = translation.getTranslation();
    FVector velocity = velocityWorld;
    FVector accelerationPart = 0.5f * accelerationWorld * deltaTime * deltaTime;

    FVector xt = x0 + velocity + accelerationPart;
    
    //verify ground truth muss auch von rädern aus gehen später.
    double groundTruth = 40.0;
    xt.Z = std::max(groundTruth, xt.Z);



    translation.setTranslation(xt);
    SetActorLocation(xt);
}


//achtung stimmt ggf nicht
//yaw rotation of wheels
void AvehicleCar::applyDeltaRotation(float deltaTime){
    //float rearFrontDistance = 500.0f; //5m ?

    if (frontRight != nullptr){
        float theta = frontRight->thetaYawRad();

        //ankathete = rotationRadius,
        //theta dreieck gegeben durch steer angle weil orthogonales dreieck
        //geg khatete ist abstand front to rear

        float tanTheta = std::tan(theta);
        if(isNotZero(tanTheta)){
            //tan(theta) = g / a
            //g = tan(theta) * a
            //a = g / tan(theta)
            float rotationRadius = rearFrontDistance / tanTheta;
            if(isNotZero(rotationRadius)){
                float angularVelocity = velocity() / rotationRadius;

                //Rotation anwenden
                float deltaYaw = angularVelocity * deltaTime;
                MMatrix deltaRotationMatrix;
                deltaRotationMatrix.yawRadAdd(deltaYaw);

                if(showScreenLog){
                    DebugHelper::showScreenMessage("wheel yaw ", theta, FColor::Yellow);
                    DebugHelper::showScreenMessage("base ", rearFrontDistance, FColor::Yellow);
                    DebugHelper::showScreenMessage("delta yaw ", deltaYaw, FColor::Yellow);
                }
                

                MMatrix result = rotation * deltaRotationMatrix; //<-- lese richtung
                FRotator rotator = result.extractRotator();
                SetActorRotation(rotator);
            }
        }
    }
}




bool AvehicleCar::isNotZero(float value){
    return std::abs(value) > 0.001f;
}
