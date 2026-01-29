#include "wheel.h"

#include "DebugPlugin/DebugHelper.h"

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
            

            return spawned;
        }
    }

    return nullptr;
}

Awheel::Awheel() : AActor(){
    maxSteerAngleRad = MMatrix::degToRadian(30.0f);
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

void Awheel::disableTraction(){
    tractionEnabled = false;
}

void Awheel::BeginPlay(){
    tractionEnabled = true;
    Super::BeginPlay();
}


void Awheel::Tick(float deltatime){
    Super::Tick(deltatime);
}

void Awheel::addYaw(float angleRad){

    angleRad = clampRotationAdd(angleRad);
    rotation.yawRadAdd(angleRad);
    yawAngle += angleRad;
    

    if(debugPrint) DebugHelper::showScreenMessage("YAW ", (float)yawAngle);

    FRotator r = rotation.extractRotator();
    SetActorRotation(r);
}

float Awheel::clampRotationAdd(float yawAdd){
    float result = yawAngle + yawAdd;
    if(result > maxSteerAngleRad){
        yawAdd = yawAdd - (result - maxSteerAngleRad);
        return yawAdd;
    }
    if(result < -1.0f * maxSteerAngleRad){
        yawAdd = yawAdd - (result - (-1.0f *maxSteerAngleRad));
        return yawAdd;
    }

    return yawAdd;
}

void Awheel::copyRotation(Awheel *ptr){
    if(ptr != nullptr){
        yawAngle = ptr->yawAngle;
        rotation = ptr->rotation;
        SetActorRotation(ptr->GetActorRotation());
    }
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
    angularAcceleration = radPerSecond;

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


/// apply rotation after vehicle has moved (for front wheels)
void Awheel::processVelocityFromVehicleSpace(FVector &deltaDirection){
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
    s / radius = angularVelocity <--kopieren--
    
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

FVector Awheel::AllForces(
    FVector &normal, 
    float slipAngle, 
    float massPerWheel,
    FVector carLocalVelocity
){
    carLocalVelocity = moveToLocalRotationSpace(carLocalVelocity); //make local for friction force

    FVector sum =
        tractionForce() +
        corneringForce(slipAngle) +
        normalForce(normal, massPerWheel) +
        frictionForceRoadDry(normal, carLocalVelocity, massPerWheel);

    return sum;
}

/*
antriebskraft nach vorne
*/
FVector Awheel::tractionForce(){
    if(!tractionEnabled){
        return FVector(0, 0, 0);
    }

    //f traction = Torque / radiusWheel    //-->angeblich

    //Winkelbeschleunigung 
    //angularAcceleration = torque / Interia
    //torque = angularAcceleration * Interia
    
    float wheelInertia = 1.0f; //nur grade aus 
    float torque = angularAcceleration * wheelInertia; // wheelInertia = float
    float forceX = torque / radius;
    FVector force(forceX, 0, 0);

    if(debugPrint) DebugHelper::showScreenMessage("traktion force", force);

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

    FVector lateralForceDirection = FVector::CrossProduct(forward, FVector(0, 0, 1)); //orthogonal zu up und forward
    lateralForceDirection = lateralForceDirection.GetSafeNormal();


    //cornering stiffness [in N/rad] (z. B. 5000–150000) was ist das ?
    float stiffness = 5000.0f;

    //warum * -1 ?:
    //entgegen wirken der reifen rotation, das ist das driften
    //scheinbar
    FVector corneringForce = -1.0f * stiffness * slipAngle * lateralForceDirection; 

    if(debugPrint) DebugHelper::showScreenMessage("cornering ", corneringForce);
    return corneringForce;

    //return FVector(0, 0, 0);
}




FVector Awheel::frictionForceRoadDry(
    FVector &normal, 
    FVector &velocity,
    float mass
){
    float haftreibungszahlStrasseTrocken = 0.6f; //Gummi auf Beton
    //trocken 0,6 – 0,9
    //nass 0,4 – 0,6
    return frictionForce(
        normal, 
        haftreibungszahlStrasseTrocken, 
        velocity,
        mass
    );
}

FVector Awheel::frictionForceRoadWet(
    FVector &normal, 
    FVector &velocity,
    float mass
){
    float haftreibungszahlStrasseNass = 0.4f;
    return frictionForce(
        normal, 
        haftreibungszahlStrasseNass, 
        velocity,
        mass
    );
}

FVector Awheel::normalForce(FVector &normal, float mass){
    FVector fNormal = normal * normalForceMagnitude(normal, mass);

    if(debugPrint) DebugHelper::showScreenMessage("normal force", fNormal);

    return fNormal;
}

float Awheel::normalForceMagnitude(FVector &normal, float mass){
    FVector gravity(0, 0, -981);
    FVector fgravity = gravity * mass; //fg = m * g
    float dotproduct = FVector::DotProduct(normal, fgravity * -1.0f); //normal force magnitude
    return dotproduct;
}

FVector Awheel::frictionForce(
    FVector &normal, 
    float haftReibungszahl,
    FVector &velocity,
    float mass
){ //must be normalized
    if(velocity.Size() < 1.0f){
        return FVector(0, 0, 0);
    }

    // Gleit-/Haftreibungsrichtung: Tangential zur Oberfläche, entgegen Bewegungsrichtung
    FVector tangentialVelocity = velocity - FVector::DotProduct(velocity, normal) * normal;
    FVector frictionDir = -1.0f * tangentialVelocity.GetSafeNormal();
    FVector frictionForce = haftReibungszahl * frictionDir;

    if(debugPrint) DebugHelper::showScreenMessage("frictionForce ", frictionForce);

    return frictionForce;
}
