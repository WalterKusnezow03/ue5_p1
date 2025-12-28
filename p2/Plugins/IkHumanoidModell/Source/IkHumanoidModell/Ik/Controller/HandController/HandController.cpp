#include "HandController.h"

#include "IkHumanoidModell/actor/debugLimbs/CubeLimbMaker.h"
#include "IkHumanoidModell/Ik/Controller/Properties/LimbProperties.h"


HandController::HandController(){
    bHasTargetSetup = false;

    FVector offsetDebug(5, 0, 5);
    debugOffset.setTranslation(offsetDebug);

    //orientation.pitchRadAdd(-90); //up
}

HandController::~HandController(){

}

void HandController::getActors(TArray<AActor *> &outArray){
    for (int i = 0; i < fingers.Num(); i++){
        BoneAttachment &current = fingers[i];
        current.getActors(outArray);
    }
    if(palm){
        outArray.Add(palm);
    }
    
}

void HandController::setup(UWorld *world, EArmType type){
    worldPointer = world;
    typeSaved = type;

    float lengthA = 3.0f;
    float lengthB = 2.0f;
    float widthFinger = 1.0f;
    LimbProperties::GetSizeFingers(lengthA, lengthB, widthFinger);

    float forwardFingerOffset = 0.0f;
    LimbProperties::GetFingerForwardOffset(forwardFingerOffset);

    bool bLeftHand = type == EArmType::ELeft;

    fingers.SetNum(5);

    //x is forward, y right
    TArray<FVector> offsets = {
        FVector(forwardFingerOffset, -2.0f, 0.0f), //thumb
        FVector(forwardFingerOffset, -1.0f, 0.0f),
        FVector(forwardFingerOffset, 0.0f, 0.0f),
        FVector(forwardFingerOffset, 1.0f, 0.0f),
        FVector(forwardFingerOffset, 2.0f, 0.0f)
    };
    for (int i = 0; i < fingers.Num(); i++){
        FingerBoneAttachment &current = fingers[i];
        if(i < offsets.Num()){
            FVector &offset = offsets[i];
            if(bLeftHand){
                offset.Y *= -1.0f;
            }

            current.setupBone(lengthA, lengthB, worldPointer, offset, widthFinger);
        }
        
    }
    CreatePalm(world);
}

void HandController::CreatePalm(UWorld *world){
    if(!palm){
        float x = 8;
        float y = 8;
        float height = 4;
        LimbProperties::GetSizeHand(x, y, height);
        palm = CubeLimbMaker::createLimbPivotAtTop(x, y, height, world);
    }
    
}



void HandController::Tick(MMatrix &transform, float deltatime){
    //M = external * orientation <--lese richtung--
    
    /*
    //default
    MMatrix M = transform * orientation;
    MMatrix inner = orientation * debugOffset; //<--lese richtung (R * T)
    M = transform * inner; //M = N * R * T
    */

    //

    //ignore transform rotation passed.
    FVector l = transform.getTranslation();
    MMatrix t(l);
    MMatrix inner = orientation * debugOffset; //<--lese richtung, erts local move, dann drehen.
    MMatrix M = t * inner;
    //DrawAxis(orientation, l);

    if(bHasTargetSetup){
        TickForwardKinematic(M, deltatime);
    }else{
        TickNone(M, deltatime);
    }

    TickPalm(M);
}

void HandController::DrawAxis(MMatrix &orientationIn, FVector &location){
    FVector x(5, 0, 0);
    FVector y(0, 5, 0);
    FVector z(0, 0, 5);

    x = orientationIn * x;
    y = orientationIn * y;
    z = orientationIn * z;

    FVector s = location;
    DebugHelper::showLineBetween(worldPointer, s, s + x, FColor::Red, 0.3f);
    DebugHelper::showLineBetween(worldPointer, s, s + y, FColor::Green, 0.3f);
    DebugHelper::showLineBetween(worldPointer, s, s + z, FColor::Blue, 0.3f);
}


void HandController::TickAutoBasedOnTarget(MMatrix &transform, float deltatime){
    for (int i = 0; i < fingers.Num(); i++){
        FingerBoneAttachment &current = fingers[i];
        current.TickAutoBasedOnTarget(transform, deltatime);
    }
}

void HandController::TickNone(MMatrix &transform, float deltatime){
    for (int i = 0; i < fingers.Num(); i++){
        BoneAttachment &current = fingers[i];
        current.TickNone(transform, deltatime);
    }
}

void HandController::TickForwardKinematic(MMatrix &transform, float deltatime){
    for (int i = 0; i < fingers.Num(); i++){
        BoneAttachment &current = fingers[i];
        current.TickForwardKinematic(transform, deltatime);
    }
}

void HandController::TickPalm(MMatrix &transform){
    if(palm){
        FVector location = transform.getTranslation();
        FRotator r = transform.extractRotator();

        palm->SetActorLocation(location);

        //deubg block rotation for now.
        //palm->SetActorRotation(orientation.extractRotator());
    }
}

void HandController::Update(IIkCarryInterface *item){
    if(item){
        CarriedItemPositionData &data = item->getItemPositionDataRef();
        Update(data);
    }
}

void HandController::Update(CarriedItemPositionData &data){
    //single apply to data
    
    TArray<FingerTargetPair *> targets = data.GetFingerTargets(typeSaved);
    if(targets.Num() == fingers.Num()){
        for (int i = 0; i < fingers.Num(); i++){
            FingerBoneAttachment &current = fingers[i];
            if(FingerTargetPair *ptr = targets[i]){
                current.OverrideTarget(*ptr);
            }
        }
        bHasTargetSetup = true;
    }

    //orientation = data.GetRotationMatrix(typeSaved);
    FQuat q = data.quatRotation(typeSaved);
    orientation.setRotation(q);

    if (palm)
    {
        palm->SetActorRotation(q);
    }
}