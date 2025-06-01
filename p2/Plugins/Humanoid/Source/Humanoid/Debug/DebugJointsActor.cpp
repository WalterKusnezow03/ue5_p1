#include "DebugJointsActor.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/DebugHelper.h"

void ADebugJointsActor::CreateInstance(UWorld *world){
    if(world != nullptr){

        UClass *toSpawn = ADebugJointsActor::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            FVector location(0,0,0);
            ADebugJointsActor *spawned = world->SpawnActor<ADebugJointsActor>(
                toSpawn, 
                location, 
                FRotator::ZeroRotator,
                SpawnParams
            );
        }
    }
}


void ADebugJointsActor::BeginDestroy(){
    
    //delete created joints
    for (int i = 0; i < createdJoints.Num(); i++){
        Joint *current = createdJoints[i];
        delete current; 
    }

    Super::BeginDestroy();
}

ADebugJointsActor::ADebugJointsActor() : AActor(){
    PrimaryActorTick.bCanEverTick = true;
}

void ADebugJointsActor::BeginPlay(){
    Super::BeginPlay();
    initChain();
    timerFortick.Begin(3.0f, true); //auto reset
    targetLocalPos = FVector(100, 0, 0);
}

void ADebugJointsActor::initChain(){
    TArray<FVector> lengths = {
        FVector(0, 0, -50),
        FVector(0, 0, -50),
        FVector(0, 0, -50),
    };
    Joint *parent = nullptr;
    for (int i = 0; i < lengths.Num(); i++)
    {
        FVector &currentTranslation = lengths[i];
        Joint *newJoint = new Joint(GetWorld(), currentTranslation);
        createdJoints.Add(newJoint);
        if(parent != nullptr){
            parent->SetChild(newJoint);
        }
        parent = newJoint;
    }
}

void ADebugJointsActor::Tick(float deltaTime){
    Super::Tick(deltaTime);
    debugTvonWFunction();

    if(createdJoints.Num() > 0){
        Joint *root = createdJoints[0];
        if (root != nullptr)
        {

            root->Tick(deltaTime, angularVelocity, linearVelocity);

            MMatrix transform;
            FVector translation(1000, -1000, 300);
            transform.setTranslation(translation);
            root->Build(transform);

            DebugHelper::showLineBetween(
                GetWorld(),
                FVector(0, 0, 0),
                translation,
                FColor::Green,
                deltaTime * 2.0f
            );

            if(true){
                //debug change target
                angularVelocity = FVector(0, 0, 0);
                timerFortick.Tick(deltaTime);
                if (timerFortick.timesUp() == false){
                    angularVelocity = FVector(1, 0, 0);
                }
            }


            if(false){
                //debug
                MMatrix end = root->endTransform(); //goes trough all childs
                findErrorTwist(
                    transform,
                    end,
                    targetLocalPos,
                    deltaTime
                );



                //debug change target
                timerFortick.Tick(deltaTime);
                if (timerFortick.timesUp() == true)
                {
                    MMatrix mat;
                    mat.yawRadAdd(MMatrix::degToRadian(45));
                    targetLocalPos = mat * targetLocalPos;
                }
            }
            
        }
    }
    
}


void ADebugJointsActor::findErrorTwist(
    MMatrix &root, 
    MMatrix &end, 
    FVector &targetLocal,
    float deltaTime
){
    //Terror = end * root^-1
    MMatrix inverse = root.jordanInverse();

    //M = A * B * C
    //M^-1 = C^-1 * B^-1 ...
    MMatrix transformLocal = end * inverse; //ja so richtig scheinbar
    MMatrix transformTargeted(targetLocal);
    MMatrix tLocalnverse = transformLocal.jordanInverse();
    MMatrix errorTransform = transformTargeted * tLocalnverse;



    MMatrix debugApplied = errorTransform * end; //richtig
    FVector localErrorLocation = errorTransform.getTranslation();

    //aber abnormale werte bei print
    DebugHelper::showScreenMessage("errorTranslation ", localErrorLocation, FColor::Yellow); 
    DebugHelper::showLineBetween(
        GetWorld(),
        end.getTranslation(),
        debugApplied.getTranslation(),
        FColor::Purple,
        deltaTime * 2.0f
    );

    // show rotator
    MMatrix pure = errorTransform;
    pure.setTranslation(0,0,0);
    FVector forward(100, 0, 0);
    forward = pure * forward;
    DebugHelper::showLineBetween(
        GetWorld(),
        end.getTranslation(),
        end.getTranslation() + forward, //zeigt einfach nach vorne ? 
        FColor::Orange,
        deltaTime * 2.0f
    );





    FVector errorAngularVelocity(0,0,99); //debug werte
    FVector errorLinearVelocity(0,0,99);

    FVector location = errorTransform.getTranslation();
    Matrix3x3 rotation;
    rotation.setRotation(errorTransform);

    float debugWantedAngularVelocity = 1;
    Matrix3x3::convertSE3ToPluecker(
        rotation,
        location,
        errorAngularVelocity,
        errorLinearVelocity,
        debugWantedAngularVelocity
    );

    DebugHelper::showScreenMessage("ErrorTwist ", errorAngularVelocity, errorLinearVelocity, FColor::Yellow);


    //unklar ob -= oder =
    angularVelocity += errorAngularVelocity;
    linearVelocity += errorLinearVelocity;
}







/**
 * IK by W testing
 */

void ADebugJointsActor::debugTvonWFunction(){
    if(createdJoints.Num() > 0){
        Joint *ptr = createdJoints[0];

        FVector target(100, 0, 0);
        solveIk(ptr, target);

        /*
        if(ptr != nullptr){
            float deltaTime = 1.0f;
            MMatrix zeroTransform;
            FVector w(1.0f, 0.0f, 0.0f);
            FVector result = ptr->T_vonW(zeroTransform, w, deltaTime);
            DebugHelper::showScreenMessage("t(w) = ", result);
        }*/
    }   
}

void ADebugJointsActor::solveIk(Joint *joint, FVector &target){
    if(joint == nullptr){
        return;
    }

    MMatrix zeroTransform;
    float deltaTime = 1.0f;
    FVector w(1.0f, 0.0f, 0.0f); //muss effizienter geraten werden, 
    //zb akuelle achse mit pitch yaw zur target achse yaw pitch w(0, y, z)

    w = guessAngularVelocity(*joint, target);
    DebugHelper::showScreenMessage("guess w: ", w, FColor::Orange);

    int tries = 10000;
    for (int i = 0; i < tries; i++){

        
        //w_tryNext = w_current - (J^-1 * t(w_current))
        //w_tryNext = w_current - b
        FVector t_w = joint->T_vonW(zeroTransform, w, deltaTime);

        float dist = FVector::Dist(t_w, target);
        if (dist < 5.0f){
            
            FString message = FString::Printf(
                TEXT("reached until 5xm with (cm: %.2f) (w : (% .2f, % .2f, % .2f))(target(% .2f, % .2f, % .2f))(reached(% .2f, % .2f, % .2f))"),
                dist,
                w.X, w.Y, w.Z,
                target.X, target.Y, target.Z,
                t_w.X, t_w.Y, t_w.Z
            );

            DebugHelper::showScreenMessage(
                message,
                FColor::Green
            );

            return;
        }

        Matrix3x3 jacobian = computeNumericalJacobian(
            zeroTransform,
            joint,
            w,
            deltaTime
        );
        Matrix3x3 jacobianInverse = jacobian.jordanInverse();
        FVector b = jacobianInverse * t_w;

        FVector w1 = w - b;
        w = w1;
    }
}

//noch sehr unklar.
Matrix3x3 ADebugJointsActor::computeNumericalJacobian(
    MMatrix &zeroTransform,
    Joint *joint, 
    const FVector& w, 
    float deltaTime
){
    if(joint == nullptr){
        Matrix3x3 result;
        return result;
    }

    float h = 1e-4f;

    /**
     * wie das hier construiert wird ist noch sehr unklar.
     * 
     * konstruiert pseudo ableitung per vektor variable
     */
    Matrix3x3 J; // jacobian
    for (int i = 0; i < 3; i++)
    {
        FVector dw = FVector::ZeroVector;
        dw[i] = h;

        FVector w1 = w + dw;
        FVector w2 = w - dw;

        FVector f_plus  = joint->T_vonW(zeroTransform, w1, deltaTime); //tFunction(w + dw);
        FVector f_minus = joint->T_vonW(zeroTransform, w2, deltaTime); //tFunction(w - dw);


        //f'(x) = dx / dy
        FVector dX_upperPart = (f_plus - f_minus);
        float OneDiv_dY_lowerPart = (1.0f / (2.0f * h));

        FVector column = dX_upperPart * OneDiv_dY_lowerPart;

        // Set column i ()
        J.setColumn(column, i);
    }

    return J;
}



FVector ADebugJointsActor::guessAngularVelocity(Joint &joint, FVector &targetLocal){
    //builded chain
    MMatrix endTransformCurrent = joint.endTransform();
    FVector endEffektorLocation = endTransformCurrent.getTranslation();


    //winkel zwischen den achsen vermessen (yaw und pitch)
    
    //yaw:
    FVector a = targetLocal.GetSafeNormal();
    FVector b = endEffektorLocation.GetSafeNormal();

    FVector2D a2D(a.X, a.Y);
    FVector2D b2D(b.X, b.Y);

    float dot = a2D.X * b2D.X + a2D.Y * b2D.Y;
    float sign = a.X * b.Y - a.Y * b.X; //normal.Z

    //cos(theta) = a dot b //wenn a und b normalisiert sind
    //acos(a dot b) = theta
    float angleYaw = sign * std::acosf(dot);

    //derotate
    MMatrix removeYaw;
    removeYaw.yawRadAdd(-angleYaw);
    a = removeYaw * a;
    b = removeYaw * b;

    //pitch
    FVector2D a2DPitch(a.X, a.Z);
    FVector2D b2DPitch(b.X, b.Z);
    dot = a2DPitch.X * b2DPitch.X + a2DPitch.Y * b2DPitch.Y;
    sign = a2DPitch.X * b2DPitch.Y - a2DPitch.Y * b2DPitch.X; //normal.Z
    float anglePitch = std::acosf(dot) * sign;

    FVector result(0, anglePitch, angleYaw);
    return result;
}