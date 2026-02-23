#include "HumanoidPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DebugPlugin/DebugHelper.h"

HumanoidPlayerController::HumanoidPlayerController(){
    weaponAimDownSight(false);
}

HumanoidPlayerController::~HumanoidPlayerController(){

}

void HumanoidPlayerController::defaultSetup(UWorld *world){
    HumanoidController::defaultSetup(world, true);
    DebugDisableCollisionOnLimbs();
    MarkEmptyActorOwnedByPlayer();
}

void HumanoidPlayerController::MarkEmptyActorOwnedByPlayer(){
    if(emptyArmTargetActor){
        emptyArmTargetActor->SetDebugPlayerAnimatedActor(true);
    }
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
    hipTranslationCopy.setTranslation(location);
    hipRotationCopy.setRotation(rotation);

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


// -- needed for diferentiating between carried weapons and empty, hand animation --
//camera if default weapon
//if hand animation: relative to hip socket, otherwise to head
MMatrix &HumanoidPlayerController::TranslationMatrixForSocket(){
    if(EmptyActorIsPickedUp()){
        return hipTranslationCopy;
    }
    return cameraWorldLocation;
}

void HumanoidPlayerController::TickMainCarriedItemSocket(float deltatime){
    /*
    The Empty Carried item including its animations are supposed to relative to the hip.
    The Default Carried items for the Player are based on the relative location 
    to the camera.
    Both Translation matrices must be supported
    -> if a carried item doesnt support rotation axis like pitch or roll
    it is eliminated with the FIKCarryInterfaceAxisConstraint info.
    */

    MMatrix &translation = TranslationMatrixForSocket();

    MMatrix rHip = hipController.getOrientation();
    mainItemSocket.Tick(
        deltatime,
        translation, //cameraWorldLocation,
        rHip,
        cameraRotationLocalPitch
    );
    /*
    ----- CAUTION BUG HERE -> SOLVED -----
    */
    
    
    /*
    mainItemSocket.Tick(
        deltatime,
        cameraWorldLocation, //deprecated, usuable if weapon attached only.
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