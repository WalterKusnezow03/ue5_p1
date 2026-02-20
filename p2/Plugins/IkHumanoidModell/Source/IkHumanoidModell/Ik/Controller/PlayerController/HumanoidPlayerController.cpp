#include "HumanoidPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DebugPlugin/DebugHelper.h"

HumanoidPlayerController::HumanoidPlayerController(){
    weaponAimDownSight(false);
}

HumanoidPlayerController::~HumanoidPlayerController(){

}

void HumanoidPlayerController::defaultSetup(UWorld *world){
    HumanoidController::defaultSetup(world);
    defaultSetupHands(world); //player only for now
    DebugDisableCollisionOnLimbs();
}

void HumanoidPlayerController::Tick(float deltatime){
    
    //not ticked for player for now.
    //hipController.Tick(deltatime);
    
    torsoController.Tick(
        //hipTDebug,
        //hipRDebug,
        hipController.getTranslation(), // MMatrix &actorTranslation,
        hipController.getOrientation(), // MMatrix &actorRotation,
        deltatime
    );
    TickMainCarriedItemSocket(deltatime);

    //shows
    //DebugHelper::showScreenMessage("HumanoidPlayerController Tick", FColor::Orange);
}

void HumanoidPlayerController::OverrideTransformAndCamera(
    UCameraComponent &camera,
    FVector &location,
    FRotator &rotation
){
    hipController.forceOverrideRotation(rotation);
    hipController.SetTranslation(location);

    //debug manual matrix copy
    rotation.Pitch = 0.0f;
    rotation.Roll = 0.0f;
    hipTDebug.setTranslation(location);
    hipRDebug.setRotation(rotation);

    UpdateAttachmentSockets(camera);

    
}

void HumanoidPlayerController::UpdateAttachmentSockets(
    UCameraComponent &camera
){
    extractRotation(camera);
    extractTranslation(camera);
}

void HumanoidPlayerController::extractRotation(UCameraComponent &camera){
    //-- Is tested, dont touch this code --
    //copy relative (local) pitch rotation
    //then accumulate with hip actor rotation
    FRotator cameraRot = camera.GetRelativeRotation(); 

    FRotator camPitchCopy;
    camPitchCopy.Pitch = cameraRot.Pitch * -1.0f; //must be flipped for reasons but its true.
    cameraRotationLocalPitch = camPitchCopy;

    /*MMatrix pitchMat(camPitchCopy);

    MMatrix rHip = hipController.getOrientation();
    cameraRotationMatrix = rHip * pitchMat;//..then accumulate with hip actor rotation*/
}

void HumanoidPlayerController::extractTranslation(UCameraComponent &camera){
    FVector location = camera.GetComponentLocation(); // world space (?)
    cameraWorldLocation.setTranslation(location); 
}

void HumanoidPlayerController::TickMainCarriedItemSocket(float deltatime){
    MMatrix rHip = hipController.getOrientation();
    mainItemSocket.Tick(
        deltatime,
        cameraWorldLocation,
        rHip,
        cameraRotationLocalPitch
    );
    
    /*
    mainItemSocket.Tick(
        deltatime,
        cameraWorldLocation,
        cameraRotationMatrix
    );*/
}


void HumanoidPlayerController::weaponAimDownSight(bool aiming){
    if(bDebugBlockPlayerApiCarriedItem){
        return;
    }

    //dynamic blocking
    if(readyRaisedPositionFlagged && !aiming){
        //not aiming->hip fire, sprint: no update
        return;
    }

    
    if(bLogMessage){
        FString msg = aiming ? TEXT("AIM") : TEXT("HIP");
        DebugHelper::showScreenMessage("HumanoidPlayerController: switch", msg);
    }
    

    //REFACTURE Tmp here
    FVector ads(50, 0, 0);
    FVector hip(50, 20, -10);
    float timeOfAnimation = 0.5f;

    FVector locationUpdate = aiming ? ads : hip;
    FRotator none;

    mainItemSocket.MoveToLocal(locationUpdate, timeOfAnimation);
    mainItemSocket.RotateToLocal(none, timeOfAnimation);
}


void HumanoidPlayerController::weaponRaisedReadyPosition(bool raise){

    if(bDebugBlockPlayerApiCarriedItem){
        return;
    }
    if(!raise){
        readyRaisedPositionFlagged = false;
        weaponAimDownSight(false);
        return;
    }
    if(bLogMessage){
        DebugHelper::showScreenMessage("HumanoidPlayerController: switch SPRINT");
    }
    

    //REFACTURE Tmp here
    FVector hip(50, 20, -10);
    FRotator pitched;
    pitched.Pitch += 70.0f;
    float timeOfAnimation = 0.5f;

    mainItemSocket.MoveToLocal(hip, timeOfAnimation);


    bool blockPerformance = true;
    if(blockPerformance){
        mainItemSocket.RotateToLocal(pitched, timeOfAnimation);
        return;
    }
}

void HumanoidPlayerController::weaponContactPosition(){
    weaponAimDownSight(false);
}




void HumanoidPlayerController::DebugDisableCollisionOnLimbs(){
    TArray<AActor *> actors = actorInChildrenArray();
    for (int i = 0; i < actors.Num(); i++){
        if(AActor *current = actors[i]){
            AcustomMeshActorBase *casted = Cast<AcustomMeshActorBase>(current);
            if(casted){
                casted->disablePhysicscollision();
            }
            /*
            if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(current->GetRootComponent()))
            {
                RootPrim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }*/
        }
    }
}