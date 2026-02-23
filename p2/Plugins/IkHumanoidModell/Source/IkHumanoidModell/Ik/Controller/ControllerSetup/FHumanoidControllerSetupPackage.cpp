#include "FHumanoidControllerSetupPackage.h"

FHumanoidControllerSetupPackage FHumanoidControllerSetupPackage::GetDefault(UWorld *world){
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

    return newPackage;
}

FHumanoidControllerSetupPackage::FHumanoidControllerSetupPackage(UWorld *world){
    SetWorld(world);
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

        worldPtr = other.worldPtr;
    }
    return *this;
}


void FHumanoidControllerSetupPackage::SetWorld(UWorld *world){
    FAbstractProperty::SetWorld(world);
    armSize.SetWorld(world);
    legPropertyLeft.SetWorld(world);
    legPropertyRight.SetWorld(world);
    torsoSize.SetWorld(world);
    handSize.SetWorld(world);
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




void FHumanoidControllerSetupPackage::MarkHandsWanted(){
    handWanted = true;
}

bool FHumanoidControllerSetupPackage::HandsMarkedWanted(){
    return handWanted;
}

FHandProperty &FHumanoidControllerSetupPackage::GetHandSize(){
    return handSize;
}
