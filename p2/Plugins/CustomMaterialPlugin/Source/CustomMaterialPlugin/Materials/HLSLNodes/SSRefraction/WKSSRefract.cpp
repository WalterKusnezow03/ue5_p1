#include "WKSSRefract.h"

#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKSSRefract::SetupInputsOnConstruct(){
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
        customExpression->Inputs.Add(FCustomInput{TEXT("Normal")});
        customExpression->Inputs.Add(FCustomInput{TEXT("ViewDir")});
        customExpression->Inputs.Add(FCustomInput{TEXT("IOR")});

        //current stencil
        customExpression->Inputs.Add(FCustomInput{TEXT("Stencil")});

        //target stencil
        customExpression->Inputs.Add(FCustomInput{TEXT("TargetStencil")});
    }
}

void UWKSSRefract::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float2 für UV
    if(customExpression){
        customExpression->OutputType = CMOT_Float2;//CMOT_Float4;
    }
    
}


void UWKSSRefract::SetupInternalExpressionsOnConstruct()
{
    if (customExpression)
    {
        // //epsilon = 0.01;
        customExpression->Code = TEXT(R"(
            //if(Stencil != TargetStencil){
            //    return float2(0,0);
            //}
            if (abs(Stencil - TargetStencil) > 1.0){
                return float2(0,0);
            }
            //debug
            bool debug = true;
            if(debug){
            
            }



            
            float3 N = normalize(Normal.xyz);
            float3 V = normalize(ViewDir);


            // -- refraction direction --
            float cosi = clamp(dot(-V, N), -1.0, 1.0);
            float etai = 1.0;
            float etat = IOR;
            float3 n = N;

            if (cosi > 0)
            {
                float tmp = etai;
                etai = etat;
                etat = tmp;
                n = -N;
            }

            float etaRatio = etai / etat;
            float k = 1.0 - etaRatio * etaRatio * (1.0 - cosi * cosi);
            float3 refrDir = (k < 0) ? float3(0,0,0) : etaRatio * V + (etaRatio * cosi - sqrt(k)) * n;
            
            // Kamera Space Annahme: ViewDir already in view space
            float2 offset = refrDir.xy / max(refrDir.z, 0.001);
            return offset;

        )");
    }
}

int32 UWKSSRefract::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}


#endif



