#pragma once

#include "CoreMinimal.h"
#include "FAbstractProperty.h"
#include "DebugPlugin/DebugHelper.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"

class IKHUMANOIDMODELL_API FSingleLimbProperty : public FAbstractProperty{

public:
    
    
    float GetSize(){
        return sizeDepth;
    }

    float GetWidth(){
        return widthOptional;
    }

    void Setup(float depth){
        sizeDepth = Validate(depth);
    }

    void Setup(float depth, float width){
        
        sizeDepth = Validate(depth);
        widthOptional = Validate(width);
    }

    void DebugLogMessage(){
        DebugLogMessage(TEXT(""));
    }
    void DebugLogMessage(FString prefix){
        prefix += FString::Printf(TEXT("FSingleLimbProperty Setup(%.2f %.2f)"), sizeDepth, widthOptional);
        DebugHelper::logMessage(prefix);
    }

    materialEnum GetLimbMaterial(){
        return materialtype;
    }

    void SetLimbMaterial(materialEnum typeIn){
        materialtype = typeIn;
    }

protected:
    float sizeDepth = 1.0f;
    float widthOptional = 1.0f;
    materialEnum materialtype = materialEnum::wallMaterial;
};
