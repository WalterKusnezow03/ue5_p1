#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FTwoLimbProperty.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FHandProperty.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FAbstractProperty.h"

class IKHUMANOIDMODELL_API FHumanoidControllerSetupPackage : public FAbstractProperty{

public:
    static FHumanoidControllerSetupPackage GetDefault(AActor *actor);

protected:
    static void DefaultTorsoSetup(FHumanoidControllerSetupPackage &newPackage, int widthMainBones);
    static void DefaultLegSetup(FHumanoidControllerSetupPackage &newPackage, int widthMainBones);

    static void DefaultHandSetup(FHumanoidControllerSetupPackage &newPackage);

public:
    FHumanoidControllerSetupPackage(AActor *actor);
    ~FHumanoidControllerSetupPackage();

    FHumanoidControllerSetupPackage(const FHumanoidControllerSetupPackage &other);
    FHumanoidControllerSetupPackage &operator=(const FHumanoidControllerSetupPackage &other);

    virtual void SetActor(AActor *actor) override;

    FTwoLimbProperty &GetArmSize();
    FTwoLimbProperty &GetTorsoSize();
    FTwoLimbProperty &GetTorsoSpineSize();

    FTwoLimbHipProperty &GetLegLeft();
    FTwoLimbHipProperty &GetLegRight();

    FLocomotionProperty &GetLocomotionProperty();

    void MarkHandsWanted();
    bool HandsMarkedWanted();
    FHandProperty &GetHandSize();

private:
    FLocomotionProperty locoMotionProperty;

    FTwoLimbProperty armSize;
    FTwoLimbHipProperty legPropertyLeft;
    FTwoLimbHipProperty legPropertyRight;

    
    //spine
    FTwoLimbProperty torsoSpineSize;

    //breast
    FTwoLimbProperty torsoSize;

    bool handWanted = false;
    FHandProperty handSize;
};
