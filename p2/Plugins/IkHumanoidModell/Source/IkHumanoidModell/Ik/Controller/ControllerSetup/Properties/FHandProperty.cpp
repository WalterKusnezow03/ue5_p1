#include "FHandProperty.h"


void FHandProperty::GenerateFingerProperties(
    TArray<FHandFingerProperty> &outArray,
    int numFingers,
    EArmType typeHand
){
    if(numFingers > 0){
        TArray<FVector> positions;
        GenerateAllFingerPositions(positions, numFingers, typeHand);

        outArray.SetNum(numFingers);
        for (int i = 0; i < positions.Num(); i++)
        {
            FHandFingerProperty &current = outArray[i];
            current.setup(fingerSize, positions[i], GetActor());
        }
    }
}

void FHandProperty::GenerateAllFingerPositions(
    TArray<FVector> &outArray, 
    int numFingers,
    EArmType typeHand
){
    if(numFingers <= 0){
        return;
    }

    //thumb
    outArray.Add(ThumbPosition(typeHand));
    numFingers--;
    GenerateTopFingerPositions(outArray, numFingers, typeHand);
}

void FHandProperty::GenerateTopFingerPositions(
    TArray<FVector> &outArray, 
    int numFingers, 
    EArmType typeHand
){
    //// ---- NOT TESTED ----
    if(numFingers <= 0){
        return;
    }

    //other fingers at top
    float sign = typeHand == EArmType::ELeft ? -1.0f : 1.0f;
    float offset = sign * widthHand * 0.5f;
    float step = widthHand / numFingers;

    float signFlippedForStepDirection = sign * -1.0f;

    for (int i = 0; i < numFingers; i++)
    {
        float posY = (signFlippedForStepDirection * step * i) + offset;
        FVector posGenerated(
            lengthHand,
            posY,
            0// up
        );
        outArray.Add(posGenerated);
    }
}



FVector FHandProperty::ThumbPosition(EArmType type){
    float sign = type == EArmType::ERight ? -1.0f : 1.0f;
    float signCenter = sign * 0.5f;

    return FVector(
        0,
        widthHand * signCenter,
        0// up
    );
}

FTwoLimbProperty &FHandProperty::GetFingerSize(){
    return fingerSize;
}

//get hand properties / setup
void FHandProperty::SetupHandBody(float length, float width, float depth){
    lengthHand = Validate(length);
    widthHand = Validate(width);
    depthHand = Validate(depth);
}

void FHandProperty::SetActor(AActor *actor){
    FAbstractProperty::SetActor(actor);
    fingerSize.SetActor(actor);
}

float FHandProperty::GetLengthHand(){
    return lengthHand;
}
float FHandProperty::GetWidthHand(){
    return widthHand;
}
float FHandProperty::GetDepthHand(){
    return depthHand;
}



