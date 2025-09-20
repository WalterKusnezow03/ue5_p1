#pragma once


#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "MaterialCompiler.h"
#include "CustomMaterialPlugin/Materials/Nodes/InputExpression/ExpressionInputNamed.h"

#include "NodeBase.generated.h"

/// @brief deriving from material expression node for materials.
UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UNodeBase : public UMaterialExpression
{
    GENERATED_BODY()


protected:

    //setup intput array
    // CPP
    virtual void PostInitProperties() override
    {
        Super::PostInitProperties();
        SetupInputsOnConstruct();
    }

    /// @brief override this method to add more inputs.
    virtual void SetupInputsOnConstruct(){
        expressionArray.Empty();
        FExpressionInputNamed newInput;
        expressionArray.Add(newInput);
    }



    UPROPERTY()
    TArray<FExpressionInputNamed> expressionArray;

public:
    
    //input example
    UPROPERTY()
    FExpressionInput FallbackInput;




#if WITH_EDITOR
    virtual FString NodeName() const {
        return TEXT("UNodeBase");
    }



    virtual void GetCaption(TArray<FString>& OutCaptions) const override {
        OutCaptions.Add(NodeName());
    }

    virtual FString GetDescription() const override {
        return TEXT("UNodeBase description");
    }




    virtual const TArray<FExpressionInput*> GetInputs() override
    {
        //return InputArray;
        TArray<FExpressionInput *> outArray;
        for (int i = 0; i < expressionArray.Num(); i++){
            FExpressionInputNamed &current = expressionArray[i];
            outArray.Add(current.ExpressionInputAsPtr());
        }
        return outArray;
    }

    virtual FExpressionInput* GetInput(int32 index) override
    {
        if(index >= 0 && index < expressionArray.Num()){
            return expressionArray[index].ExpressionInputAsPtr();
        }

        return nullptr;
    }

    virtual FName GetInputName(int32 index) const override
    {
        if(index >= 0 && index < expressionArray.Num()){
            return expressionArray[index].asFName();
        }
        return NAME_None;
    }
#endif
};