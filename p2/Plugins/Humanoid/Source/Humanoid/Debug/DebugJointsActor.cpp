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