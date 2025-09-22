#include "NodeBase.h"


void UNodeBase::PostInitProperties() 
{
    Super::PostInitProperties();
    SetupInputsOnConstruct();
    SetupOutputsOnConstruct();
}


FString UNodeBase::NodeName() const {
    return TEXT("UNodeBase");
}



void UNodeBase::GetCaption(TArray<FString>& OutCaptions) const {
    OutCaptions.Add(NodeName());
}

FString UNodeBase::GetDescription() const {
    return TEXT("UNodeBase description");
}


///----- intput ------

const TArray<FExpressionInput*> UNodeBase::GetInputs()
{
    //return InputArray;
    TArray<FExpressionInput *> outputArray;
    for (int i = 0; i < expressionInputArray.Num(); i++){
        outputArray.Add(&expressionInputArray[i]);
    }
    return outputArray;
        
}

FExpressionInput* UNodeBase::GetInput(int32 index)
{
    if(index >= 0 && index < expressionInputArray.Num()){
        return &expressionInputArray[index];
    }

    return nullptr;
}

FName UNodeBase::GetInputName(int32 index) const
{
    if(index >= 0 && index < expressionInputArray.Num()){
        return expressionInputArray[index].InputName;
    }
    return NAME_None;
}

/// ----- output ------

// Rückgabe aller Outputs
TArray<FExpressionOutput>& UNodeBase::GetOutputs() 
{
    return expressionOutputArray;
}

//Returns the output at specified index is valid
FExpressionOutput* UNodeBase::GetOutput(int32 OutputIndex){
    if(OutputIndex >= 0 && OutputIndex < expressionOutputArray.Num()){
        return &expressionOutputArray[OutputIndex];
    }
    return nullptr;
}