#pragma once

#include "CoreMinimal.h"
#include "ExpressionInputNamed.generated.h"

USTRUCT(BlueprintType) // BlueprintType erlaubt Nutzung in Blueprints
struct FExpressionInputNamed
{
    GENERATED_BODY()



public:
    
    FExpressionInputNamed(){
        name = TEXT("EmptyName");
    }

    FExpressionInputNamed(FString nameIn){
        name = nameIn;
    }

    FName asFName() const {
        return FName(name);
    }

    FExpressionInput *ExpressionInputAsPtr(){
        return &Input;
    }

    UPROPERTY()
    FExpressionInput Input;
    FString name;
};