#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FTwoLimbProperty.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FHandProperty.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FAbstractProperty.h"

class IKHUMANOIDMODELL_API FHumanoidControllerSetupPackage : public FAbstractProperty{

public:
    static FHumanoidControllerSetupPackage GetDefault(AActor *actor);

    FHumanoidControllerSetupPackage(AActor *actor);
    ~FHumanoidControllerSetupPackage();

    FHumanoidControllerSetupPackage(const FHumanoidControllerSetupPackage &other);
    FHumanoidControllerSetupPackage &operator=(const FHumanoidControllerSetupPackage &other);

    virtual void SetActor(AActor *actor) override;

    FTwoLimbProperty &GetArmSize();
    FTwoLimbProperty &GetTorsoSize();

    FTwoLimbHipProperty &GetLegLeft();
    FTwoLimbHipProperty &GetLegRight();

    void MarkHandsWanted();
    bool HandsMarkedWanted();
    FHandProperty &GetHandSize();

private:
    FTwoLimbProperty armSize;
    FTwoLimbHipProperty legPropertyLeft;
    FTwoLimbHipProperty legPropertyRight;

    //FTwoLimbProperty legSize;
    FTwoLimbProperty torsoSize;

    bool handWanted = false;
    FHandProperty handSize;
};
