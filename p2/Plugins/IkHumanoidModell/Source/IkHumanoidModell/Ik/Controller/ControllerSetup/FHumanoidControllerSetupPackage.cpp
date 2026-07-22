#include "FHumanoidControllerSetupPackage.h"

/// --- default setup ---

FHumanoidControllerSetupPackage FHumanoidControllerSetupPackage::GetDefault(AActor *world){
    return GetDefault(world, true);
}

FHumanoidControllerSetupPackage FHumanoidControllerSetupPackage::GetDefault(AActor *world, bool headVisible){
    int widthMainBones = 10;

    FHumanoidControllerSetupPackage newPackage(world);
    DefaultTorsoSetup(newPackage, widthMainBones);
    DefaultLegSetup(newPackage, widthMainBones);
    DefaultHeadSetup(newPackage, widthMainBones);
    DefaultHandSetup(newPackage);

    FSingleLimbProperty &head = newPackage.GetHeadSize();
    if(!headVisible){
        head.SetLimbMaterial(materialEnum::transparentMaterial);
    }

    return newPackage;
}







void FHumanoidControllerSetupPackage::DefaultTorsoSetup(
    FHumanoidControllerSetupPackage &newPackage,
    int widthMainBones
){
    FTwoLimbProperty &arm = newPackage.GetArmSize();
    FTwoLimbProperty &torsoSpine = newPackage.GetTorsoSpineSize();
    FTwoLimbProperty &torso = newPackage.GetTorsoSize();

    arm.Setup(40.0f, 40.0f, widthMainBones);

    torsoSpine.Setup(25.0f, 25.0f, widthMainBones);
    torso.Setup(15.0f, 30.0f, widthMainBones);

    //old
    //torso.Setup(50.0f, 30.0f, widthMainBones);
}

void FHumanoidControllerSetupPackage::DefaultLegSetup(
    FHumanoidControllerSetupPackage &newPackage,
    int widthMainBones
){
    FTwoLimbHipProperty &legLeft = newPackage.GetLegLeft();
    FTwoLimbHipProperty &legRight = newPackage.GetLegRight();
    legLeft.Setup(50.0f, 50.0f, widthMainBones);
    legRight.Setup(50.0f, 50.0f, widthMainBones);

    legLeft.SetHipOffset(20.0f);
    legRight.SetHipOffset(-20.0f);

    FLocomotionProperty &locomotion = newPackage.GetLocomotionProperty();
    locomotion.SetMaxVelocity(500.0f);
    locomotion.SetMinVelocity(200.0f);
}

void FHumanoidControllerSetupPackage::DefaultHeadSetup(
    FHumanoidControllerSetupPackage &newPackage,
    int widthMainBones
){
    float width = widthMainBones * 2.0f;
    float depth = widthMainBones * 2.3f;

    FSingleLimbProperty &head = newPackage.GetHeadSize();
    head.Setup(depth, width);

    FSingleLimbProperty &neck = newPackage.GetNeckSize();
    neck.Setup(depth * 1.1f, widthMainBones * 0.8f);
}

void FHumanoidControllerSetupPackage::DefaultHandSetup(FHumanoidControllerSetupPackage &newPackage){
    FHandProperty &hand = newPackage.GetHandSize();
    hand.SetupHandBody(6.0f, 6.0f, 3.0f);
    FTwoLimbProperty &handFinger = hand.GetFingerSize();
    handFinger.Setup(3.0f, 2.0f, 1.0f);
}


/// --- class methods ---

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
        torsoSpineSize = other.torsoSpineSize;
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
    torsoSpineSize.SetActor(actor);
    handSize.SetActor(actor);
    headSize.SetActor(actor);
    neckSize.SetActor(actor);
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

FTwoLimbProperty &FHumanoidControllerSetupPackage::GetTorsoSpineSize(){
    return torsoSpineSize;
}

FTwoLimbProperty &FHumanoidControllerSetupPackage::GetTorsoSize(){
    return torsoSize;
}

FLocomotionProperty &FHumanoidControllerSetupPackage::GetLocomotionProperty(){
    return locoMotionProperty;
}

FSingleLimbProperty &FHumanoidControllerSetupPackage::GetHeadSize(){
    return headSize;
}

FSingleLimbProperty &FHumanoidControllerSetupPackage::GetNeckSize(){
    return neckSize;
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
