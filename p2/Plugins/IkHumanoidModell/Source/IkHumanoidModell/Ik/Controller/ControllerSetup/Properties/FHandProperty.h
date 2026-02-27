#pragma once

#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FTwoLimbProperty.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
#include "FAbstractProperty.h"
#include "FHandFingerProperty.h"

class IKHUMANOIDMODELL_API FHandProperty : public FAbstractProperty{

public:
    
    FTwoLimbProperty &GetFingerSize();

    // get hand properties / setup
    void SetupHandBody(float length, float width, float depth);

    virtual void SetActor(AActor *world) override;

    float GetLengthHand();
    float GetWidthHand();
    float GetDepthHand();

    void GenerateFingerProperties(
        TArray<FHandFingerProperty> &outArray,
        int numFingers,
        EArmType typeHand
    );

private:
    FTwoLimbProperty fingerSize;

    float lengthHand = 10.0f;
    float widthHand = 10.0f;
    float depthHand = 3.0f;

    void GenerateAllFingerPositions(
        TArray<FVector> &outArray,
        int numFingers,
        EArmType typeHand
    );

    FVector ThumbPosition(EArmType type);
    void GenerateTopFingerPositions(
        TArray<FVector> &outArray,
        int numFingers,
        EArmType typeHand
    );
    
};