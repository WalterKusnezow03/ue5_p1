#include "FHumanoidControllerSetupPackage.h"

FHumanoidControllerSetupPackage FHumanoidControllerSetupPackage::GetDefault(AActor *world){
    int widthMainBones = 10;

    FHumanoidControllerSetupPackage newPackage(world);
    FTwoLimbProperty &arm = newPackage.GetArmSize();
    FTwoLimbProperty &torso = newPackage.GetTorsoSize();

    arm.Setup(40.0f, 40.0f, widthMainBones);
    torso.Setup(50.0f, 30.0f, widthMainBones);

    FHandProperty &hand = newPackage.GetHandSize();
    hand.SetupHandBody(6.0f, 6.0f, 3.0f);
    FTwoLimbProperty &handFinger = hand.GetFingerSize();
    handFinger.Setup(3.0f, 2.0f, 1.0f);
    


    FTwoLimbHipProperty &legLeft = newPackage.GetLegLeft();
    FTwoLimbHipProperty &legRight = newPackage.GetLegRight();
    legLeft.Setup(50.0f, 50.0f, widthMainBones);
    legRight.Setup(50.0f, 50.0f, widthMainBones);

    legLeft.SetHipOffset(20.0f);
    legRight.SetHipOffset(-20.0f);

    FLocomotionProperty &locomotion = newPackage.GetLocomotionProperty();
    locomotion.SetMaxVelocity(500.0f);

    return newPackage;
}

FHumanoidControllerSetupPackage::FHumanoidControllerSetupPackage(AActor *world){
    SetActor(world);
}

FHumanoidControllerSetupPackage::~FHumanoidControllerSetupPackage(){

}

FHumanoidControllerSetupPackage::FHumanoidControllerSetupPackage(
    const FHumanoidControllerSetupPackage &other
){
    if(this != &other){
        *this = other;
    }
}
FHumanoidControllerSetupPackage &FHumanoidControllerSetupPackage::operator=(
    const FHumanoidControllerSetupPackage &other
){
    if(this != &other){
        armSize = other.armSize;
        legPropertyLeft = other.legPropertyLeft;
        legPropertyRight = other.legPropertyRight;
        torsoSize = other.torsoSize;
        handSize = other.handSize;

        SetActor(other.GetActor());
    }
    return *this;
}


void FHumanoidControllerSetupPackage::SetActor(AActor *actor){
    FAbstractProperty::SetActor(actor);
    armSize.SetActor(actor);
    legPropertyLeft.SetActor(actor);
    legPropertyRight.SetActor(actor);
    torsoSize.SetActor(actor);
    handSize.SetActor(actor);
}

FTwoLimbProperty &FHumanoidControllerSetupPackage::GetArmSize(){
    return armSize;
}

FTwoLimbHipProperty &FHumanoidControllerSetupPackage::GetLegLeft(){
    return legPropertyLeft;
}

FTwoLimbHipProperty &FHumanoidControllerSetupPackage::GetLegRight(){
    return legPropertyRight;
}

FTwoLimbProperty &FHumanoidControllerSetupPackage::GetTorsoSize(){
    return torsoSize;
}

FLocomotionProperty &FHumanoidControllerSetupPackage::GetLocomotionProperty(){
    return locoMotionProperty;
}


void FHumanoidControllerSetupPackage::MarkHandsWanted(){
    handWanted = true;
}

bool FHumanoidControllerSetupPackage::HandsMarkedWanted(){
    return handWanted;
}

FHandProperty &FHumanoidControllerSetupPackage::GetHandSize(){
    return handSize;
}
