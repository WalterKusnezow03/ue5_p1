#pragma once

#include "CoreMinimal.h"


class ASSETPLUGIN_API EnumTool {


public:
    template<typename E>
	static FString EnumName(){
		UEnum* EnumPtr = StaticEnum<E>(); //isA UObject
		FString outname = TEXT("invalidEnum");
		if(EnumPtr){
			//is just as in the Header definition, the enum name.
			outname = EnumPtr->GetFName().ToString();
        }
        return outname;
    }

    template<typename E>
	static bool IsEnum(){
		UEnum* EnumPtr = StaticEnum<E>(); //isA UObject
        return EnumPtr != nullptr;
    }

    template <typename E> 
    static bool IsEnumOfName(FString nameIn){
        if(IsEnum<E>()){
            return EnumName<E>().Equals(nameIn);
        }
        return false;
    }

    template<typename E>
    static bool ExtractEnumValue(E &outValue, FString enumValueName){
        if(IsEnum<E>()){
            //index inside enum
            int64 FoundValue = StaticEnum<E>()->GetValueByName(FName(*enumValueName));
            if (FoundValue != INDEX_NONE)
            {
                // Sicher konvertieren
                outValue = static_cast<E>(FoundValue);
                return true;
            }
        }
        return false;
    }


};