#pragma once

#include "CoreMinimal.h"
#include "CustomMaterialPlugin/MaterialParametrazation/base/DynamicMaterialWrapperBase.h"
#include <map>

/// --- testing needed ---

/// @brief wrapping umaterial to abstract parameter values
/// holds the material instance dynamic, 
/// is needed for dynamic material parameters where the material
/// is applied to more than one object.
/// will also handle timing when to change a value.
///
/// Will allow to register param values by any enum and attaching a string to it (abstraction.)

template <typename T> //register names by typename T
class CUSTOMMATERIALPLUGIN_API TDynamicMaterialWrapper : public FDynamicMaterialWrapperBase {

    ///TODO: Einbinden in uprocedural mesh component pair!

protected:
    
    std::map<T, FString> parameterMap;

public:
    

    /// ---- TEMPLATE FUNCTIONS ----
    
    void SetScalarParameterValue(T type, float value){
        if(HasParameter(type)){
            SetScalarParameterValue(GetName(type), value);
        }
    }
    
    void SetVectorParameterValue(T type, const FVector &value){
        if(HasParameter(type)){
            SetVectorParameterValue(GetName(type), value);
        }
    }

    void SetTextureParameterValue(T type, UTexture *texture){
        if(HasParameter(type)){
            SetTextureParameterValue(GetName(type), texture);
        }
    }



    //template gilt für ganze klasse.
    void RegisterParameter(T t, FString s){
        parameterMap[t] = s;
    }

protected:
    bool HasParameter(T t){
        return parameterMap.find(t) != parameterMap.end();
    }

    FString GetName(T t){
        if(HasParameter(t)){
            return parameterMap[t];
        }
        return TEXT("NONE"); //nothing should hapen when setting a parameter which does not exist.
    }


};
