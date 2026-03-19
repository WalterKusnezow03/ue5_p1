#include "HeadController.h"
#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"


HeadController::HeadController(){
    neckRotation.pitchRadAdd(MMatrix::degToRadian(180.0f));
}

HeadController::~HeadController(){

}

void HeadController::setup(FHumanoidControllerSetupPackage &setupPackage){
    worldPointer = setupPackage.GetWorld();
    FSingleLimbProperty &head = setupPackage.GetHeadSize();
    FSingleLimbProperty &neck = setupPackage.GetNeckSize();
    setupJoints(head, neck);

}

void HeadController::setupJoints(
    FSingleLimbProperty &head,
    FSingleLimbProperty &neck
){
    //float width = head.GetWidth();
    //float height = head.GetSize();
    setupNeckJoints(neck.GetSize());
    setupHeadJoint(head.GetSize());
    SetupJointParents();
    SetupActorLimbs(head, neck);
}

void HeadController::setupNeckJoints(float height){
    FVector aVec(0, 0, -height);
    neckTranslation.setTranslation(aVec);
    p1 = Joint(aVec, worldPointer);
}

void HeadController::setupHeadJoint(float height){
    FVector bVec(0, 0, -height);
    p2 = Joint(bVec, worldPointer);
}

void HeadController::SetupJointParents(){
    p1.AddChildByPointer(&p2);
}


void HeadController::SetupActorLimbs(
    FSingleLimbProperty &headProperty,
    FSingleLimbProperty &neckProperty
){
    if(worldPointer){
        AActor *neck = CubeLimbMaker::createLimbPivotAtTop(
            neckProperty.GetWidth(), 
            neckProperty.GetWidth(), 
            neckProperty.GetSize(), 
            worldPointer
        );
        AActor *head = CubeLimbMaker::createLimbPivotAtTop(
            headProperty.GetWidth(), 
            headProperty.GetWidth(), 
            headProperty.GetSize(), 
            worldPointer
        );
        attachLimbs(neck, head);
    }
}



void HeadController::attachLimbs(AActor *top, AActor *bottom){
    neckActor = top;
    headActor = bottom;
    p1.SetActor(top);
    p2.SetActor(bottom);
}

void HeadController::applyTransformToActors(MMatrix &world, MMatrix &top, MMatrix &bottom){
    if(autoCreateLimbs){ //has limbs.

        //rotation always in next matrix to the prev location matrix.
        TwoJointBone::applyTransform(neckActor, world.getTranslation(), top);
        TwoJointBone::applyTransform(headActor, top.getTranslation(), bottom);
    }
    
}


void HeadController::getActors(TArray<AActor *> &outArray){
    if(neckActor){
        outArray.Add(neckActor);
    }
    if(headActor){
        outArray.Add(headActor);
    }
}




void HeadController::Tick(
    const MMatrix &actorTranslation,
    const MMatrix &actorRotation,
    float deltatime
){
    if(collapseEnabledFlag){
        return;
    }
    MMatrix MWorld = actorTranslation * actorRotation; //<-- lese richtung --
    
    MMatrix Mneck = neckRotation * neckTranslation; //<-- lese richtung -- M = R * T
    MMatrix Mhead = headTranslation * headRotation; //<-- lese richtung --

    MMatrix MneckWorld = MWorld * Mneck;
    MMatrix MheadWorld = MneckWorld * Mhead;

    applyTransformToActors(MWorld, MneckWorld, MheadWorld);
}






//connect to spine
Joint *HeadController::GetTopJoint(){
    return &p1;
}



void HeadController::ReactToDamage(const FCustomHitResult &hitResult){
    if(AActor *hitActor = hitResult.GetActor()){
        if(hitActor == neckActor){
            ReactToDamage(hitResult, p1);
        }
        if(hitActor == headActor){
            ReactToDamage(hitResult, p2);
        }
    }
}

void HeadController::ReactToDamage(
    const FCustomHitResult &hitResult,
    Joint &affectedJoint
){
    affectedJoint.ReactToDamage(hitResult);
}




void HeadController::SetHeadRotation(
    const FRotator &rotation
){
    FRotator copy = rotation;
    headRotation.setRotation(copy);
}
