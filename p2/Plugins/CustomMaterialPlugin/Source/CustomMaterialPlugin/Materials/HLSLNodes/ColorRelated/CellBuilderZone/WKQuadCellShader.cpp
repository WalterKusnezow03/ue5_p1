#include "WKQuadCellShader.h"

#if WITH_EDITOR

/// @brief override this method to add more inputs.
void UWKQuadCellShader::SetupInputsOnConstruct(){
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
        customExpression->Inputs.Add(FCustomInput{TEXT("ColorIn")});
        customExpression->Inputs.Add(FCustomInput{TEXT("localPosIn")});
        customExpression->Inputs.Add(FCustomInput{TEXT("sizeSquare")});
        customExpression->Inputs.Add(FCustomInput{TEXT("curveWidth")});
    }
    
}

void UWKQuadCellShader::SetupOutputsOnConstruct(){
    //empty for derivation override

    // OutputType: Float4 für Color
    if(customExpression){
        customExpression->OutputType = CMOT_Float4;
    }
    
}


void UWKQuadCellShader::SetupInternalExpressionsOnConstruct()
{
    if (customExpression)
    {
        // --- DOES NOT WORK ---


        // //epsilon = 0.01;
        customExpression->Code = TEXT(R"(
            //debug
            curveWidth = abs(curveWidth * 2.0);
            sizeSquare = abs(sizeSquare);
            if(curveWidth < 0){
                curveWidth = 1;
            }
            if(sizeSquare < 0){
                sizeSquare = 1;
            }
            
            localPosIn = fmod(abs(localPosIn), curveWidth);
            if(localPosIn.x < sizeSquare && localPosIn.y < sizeSquare)
            {
                return float4(ColorIn, 1.0);
            }
            return float4(0, 0, 0, 0);

        )");
    }
}

int32 UWKQuadCellShader::Compile(FMaterialCompiler *Compiler, int32 outputIndex){
    if(customExpression){
        return customExpression->Compile(Compiler, outputIndex);
    }
    return INDEX_NONE;
}







#endif