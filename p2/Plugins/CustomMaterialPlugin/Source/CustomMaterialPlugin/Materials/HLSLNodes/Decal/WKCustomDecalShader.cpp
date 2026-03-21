#include "WKCustomDecalShader.h"


#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKCustomDecalShader::SetupInputsOnConstruct(){
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
        customExpression->Inputs.Add(FCustomInput{TEXT("vertexWorld")});
        customExpression->Inputs.Add(FCustomInput{TEXT("decalWorld")});
        customExpression->Inputs.Add(FCustomInput{TEXT("decalScale")});

        
        customExpression->Inputs.Add(FCustomInput{TEXT("pitch")});
        customExpression->Inputs.Add(FCustomInput{TEXT("yaw")});
        //customExpression->Inputs.Add(FCustomInput{TEXT("decalSampler")});
    }
    
}

void UWKCustomDecalShader::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float4 für Color
    if(customExpression){
        customExpression->OutputType = CMOT_Float2;
    }
    
}


void UWKCustomDecalShader::SetupInternalExpressionsOnConstruct(){
    //empty for derivation override (using internal expressions of unreal to hide inside)

    // HLSL Code
    if(customExpression){
        customExpression->Code = TEXT(R"(
            

            float3x3 Ry = float3x3(
                cos(pitch),  0, sin(pitch),
                0,          1,       0,
            -sin(pitch), 0, cos(pitch)
            );

            float3x3 Rz = float3x3(
                cos(yaw), -sin(yaw), 0,
                sin(yaw),  cos(yaw), 0,
                0,             0,    1
            );



            
            float3x3 R = Rz * Ry;
            float3x3 R1 = transpose(R); //R^T = R^-1
            float3 localPos = mul(R1, vertexWorld - decalWorld); //AB = B - A

            // Box-Clipping
            if(abs(localPos.x) > decalScale.x*0.5 ||
            abs(localPos.y) > decalScale.y*0.5 ||
            abs(localPos.z) > decalScale.z*0.5)
            {
                return float2(-1,-1); // außerhalb: transparent
            }

            // UVs
            float2 uv = localPos.xy / decalScale.xy + 0.5;
            uv.x = 1.0 - uv.x;

            return uv;
        )");
    }
    
}

int32 UWKCustomDecalShader::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}

#endif