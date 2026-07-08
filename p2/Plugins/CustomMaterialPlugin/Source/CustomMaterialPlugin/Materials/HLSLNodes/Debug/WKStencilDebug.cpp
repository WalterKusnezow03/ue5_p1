#include "WKStencilDebug.h"

#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKStencilDebug::SetupInputsOnConstruct(){
    UMaterial* OuterMaterial = GetTypedOuter<UMaterial>(); //expression must be owned by material itself.
    if(OuterMaterial && !customExpression)
    {
        customExpression = NewObject<UMaterialExpressionCustom>(
            this,
            UMaterialExpressionCustom::StaticClass(),
            NAME_None,
            RF_Transactional
        );
    }
    if(customExpression){

        //current stencil
        customExpression->Inputs.Add(FCustomInput{TEXT("UV")});

        //target stencil
        customExpression->Inputs.Add(FCustomInput{TEXT("TargetStencil")});
    }
}

void UWKStencilDebug::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float2 für UV
    if(customExpression){
        customExpression->OutputType = CMOT_Float1;
    }
    
}


void UWKStencilDebug::SetupInternalExpressionsOnConstruct()
{
    if (customExpression)
    {
        // //epsilon = 0.01;
        customExpression->Code = TEXT(R"(
            float stencilLocal = SceneTextureLookup(UV, 14, false).r;

            if (abs(stencilLocal - TargetStencil) > 0.5){
                return 0;
            }
            return 1;
        )");
    }
}

int32 UWKStencilDebug::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}


#endif



