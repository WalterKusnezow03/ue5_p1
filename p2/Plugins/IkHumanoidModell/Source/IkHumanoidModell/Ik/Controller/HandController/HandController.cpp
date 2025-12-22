#include "HandController.h"


HandController::HandController(){
    bHasTargetSetup = false;

    FVector offsetDebug(10, 0, 0);
    debugOffset.setTranslation(offsetDebug);

    orientation.pitchRadAdd(-90); //up
}

HandController::~HandController(){

}

void HandController::getActors(TArray<AActor *> &outArray){
    for (int i = 0; i < fingers.Num(); i++){
        BoneAttachment &current = fingers[i];
        current.getActors(outArray);
    }
}



void HandController::setup(UWorld *world, EArmType type){
    worldPointer = world;
    typeSaved = type;

    float lengthA = 3.0f;
    float lengthB = 2.0f;
    float widthFinger = 1.0f;

    bool bLeftHand = type == EArmType::ELeft;

    fingers.SetNum(5);

    //x is forward, y right
    TArray<FVector> offsets = {
        FVector(2.0f, -3.0f, 0.0f), //thumb
        FVector(4.0f, -1.0f, 0.0f),
        FVector(4.0f, 0.0f, 0.0f),
        FVector(4.0f, 1.0f, 0.0f),
        FVector(4.0f, 2.0f, 0.0f)
    };
    for (int i = 0; i < fingers.Num(); i++){
        BoneAttachment &current = fingers[i];
        if(i < offsets.Num()){
            FVector &offset = offsets[i];
            if(bLeftHand){
                offset.Y *= -1.0f;
            }

            current.setupBone(lengthA, lengthB, worldPointer, offset, widthFinger);
        }
        
    }

}



void HandController::Tick(MMatrix &transform, float deltatime){
    //M = external * orientation <--lese richtung--
    
    //default
    MMatrix M = transform * orientation;

    //debugoffset
    MMatrix inner = orientation * debugOffset; //<--lese richtung (R * T)
    M = transform * inner;

    if(bHasTargetSetup){
        TickForwardKinematic(M, deltatime);
    }else{
        TickNone(M, deltatime);
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



void HandController::Update(IIkCarryInterface *item){
    if(item){
        CarriedItemPositionData &data = item->getItemPositionDataRef();
        Update(data);
    }
}

void HandController::Update(CarriedItemPositionData &data){
    //single apply to data
    TArray<FVector>& targets = data.GetFingerTargetsLocal(typeSaved);
    if(targets.Num() == fingers.Num()){
        for (int i = 0; i < fingers.Num(); i++){
            BoneAttachment &current = fingers[i];
            FVector &targetLocal = targets[i];
            current.setForwardTargetLocal(targetLocal);
        }
        bHasTargetSetup = true;
    }
}