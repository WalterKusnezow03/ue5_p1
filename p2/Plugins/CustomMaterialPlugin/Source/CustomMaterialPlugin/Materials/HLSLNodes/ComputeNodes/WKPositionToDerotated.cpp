#include "WKPositionToDerotated.h"

#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKPositionToDerotated::SetupInputsOnConstruct(){
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
        customExpression->Inputs.Add(FCustomInput{TEXT("localPosIn")});
        customExpression->Inputs.Add(FCustomInput{TEXT("normalIn")});
        customExpression->Inputs.Add(FCustomInput{TEXT("tangentIn")});
    }
    
}

void UWKPositionToDerotated::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float4 für Color
    if(customExpression){
        customExpression->OutputType = CMOT_Float3;
    }
    
}


void UWKPositionToDerotated::SetupInternalExpressionsOnConstruct()
{
    if (customExpression)
    {
        // //epsilon = 0.01;
        customExpression->Code = TEXT(R"(
            float3 side = normalize(tangentIn);
            float3 up = normalize(normalIn);
            float3 forward = cross(up, side);
    
            //sollte x = side, y = up, z = forward sein
            float3x3 R = float3x3(side, up, forward); //SUF //float3x3(side, up, forward);


            float3x3 RT = transpose(R);
            float3 p_local = mul(RT, localPosIn);
            p_local.z = 0.0;

            return p_local;
        )");
    }
}

int32 UWKPositionToDerotated::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}







#endif