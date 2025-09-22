#include "WKPixelShaderBase.h"

// --- output ---
void UWKPixelShaderBase::SetupOutputsOnConstruct(){
    Super::SetupOutputsOnConstruct();
    FExpressionOutput pixelOutput;
    pixelOutput.OutputName = FName(TEXT("PixelColorOutput"));
    expressionOutputArray.Add(pixelOutput); 
}



// --- hepers ---
int32 UWKPixelShaderBase::GetScreenPositionAsPixel(FMaterialCompiler* Compiler)
{
    return Compiler->GetPixelPosition();
}

int32 UWKPixelShaderBase::GetScreenPositionAsUV(FMaterialCompiler *Compiler){
    return Compiler->GetViewportUV();
}

int32 UWKPixelShaderBase::GetScreenPoisitionAsUvRelativeToCenter(FMaterialCompiler *Compiler){
    int32 center = MakeConstant2D(Compiler, 0.5f, 0.5f);
    int32 pos = GetScreenPositionAsUV(Compiler);
    //AB = B - A --> center to screen
    return Compiler->Sub(pos, center);
}

int32 UWKPixelShaderBase::RadialScalarFromScreenCoordinateRelativeToCenter(FMaterialCompiler *Compiler){
    int32 uvRelativeToCenter = GetScreenPoisitionAsUvRelativeToCenter(Compiler); //between 0 and 0.5
    int32 uvRelativeToCenterNormalized = Compiler->Mul(
        uvRelativeToCenter, 
        MakeConstant(Compiler, 2.0f) //to make between 0 and 1 scaled
    );

    int32 X = Compiler->ComponentMask(uvRelativeToCenterNormalized, true, false, false, false);
    int32 Y = Compiler->ComponentMask(uvRelativeToCenterNormalized, false, true, false, false);

    //size vector between 0 and 1
    return SizeVector2D(Compiler, X, Y);
}



// --- compile ---
int32 UWKPixelShaderBase::Compile(FMaterialCompiler *Compiler, int32 OutputIndex){
    return CompilePixelShader(Compiler);
}

//called by compile
int32 UWKPixelShaderBase::CompilePixelShader(FMaterialCompiler *Compiler){
    return INDEX_NONE;
}
