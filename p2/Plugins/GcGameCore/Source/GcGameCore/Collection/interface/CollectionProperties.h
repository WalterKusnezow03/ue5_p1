#pragma once 

#include "CoreMinimal.h"

/// @brief will collect identification info to track objects by class name and additional keys
class GCGAMECORE_API CollectionProperties {

private:
    bool bLogEnabled = true;
    TArray<FString> keys;
    UClass *type = nullptr;

public:
    UClass *getType(){
        return type;
    }

    bool HasNextKey(int index){
        return index < keys.Num();
    }

    FString GetKey(int index){
        if(index >= 0 && index < keys.Num()){
            return keys[index];
        }
        return TEXT("INVALID");
    }

    void SetFirstKey(UClass *someStaticClass){
        if(someStaticClass){
            type = someStaticClass;
            FString Name = someStaticClass->GetFName().ToString();
            if(keys.Num() <= 0){
                keys.Add(Name);
            }else{
                // vorne einfügen
                //array.Insert(<val>, <index>);
                keys.Insert(Name, 0);
            }

            //classKey = Name;
        }
    }

    
    void SetFirstKey(UObject *obj){ //Actor is also isA uobject
        if (IsValid(obj)) // prüft != nullptr und !PendingKill
        {
            UClass* classPtr = obj->GetClass();
            SetFirstKey(classPtr);
        }
    }

    template<typename E> 
    void AddEnumProperty(E e){
        FString nameEnum = EnumName<E>();
        int32 key = EnumKey<E>(e);
        if(nameEnum != TEXT("invalidEnum") && key != -1){
            FString newKey = FString::Printf(TEXT("%s%d"), *nameEnum, key);
            if(!keys.Contains(newKey)){
                keys.Add(newKey);
            }
        }
    }

private:
    template<typename E>
    int32 EnumKey(E e){
        UEnum* EnumPtr = StaticEnum<E>();
        if (EnumPtr){
            for(int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i){
                E current = static_cast<E>(EnumPtr->GetValueByIndex(i));
                if(current == e){
                    return i;
                }
            }
        }
        return -1;
    }

    template<typename E>
	FString EnumName(){
		UEnum* EnumPtr = StaticEnum<E>(); //isA UObject
		FString outname = TEXT("invalidEnum");
		if(EnumPtr){
			//is just as in the Header definition, the enum name.
			outname = EnumPtr->GetFName().ToString();

			if(bLogEnabled){
				FString message = FString::Printf(TEXT("collected properties Tracked Enum As String : %s"), *outname);
				UE_LOG(LogTemp, Log, TEXT("%s"), *message);
			}
			
		}
		return outname;
	}



public:
    void ShowKey(FString prefix){
        FString message = prefix + TEXT("CollectedPropertiesKey:"); 

        // FString::Printf(TEXT("collected properties Tracked Enum As String : %s"), *outname);

        for (int i = 0; i < keys.Num(); i++){
            message += TEXT(" ");
            message += keys[i];
        }

        UE_LOG(LogTemp, Log, TEXT("%s"), *message);
    }



    void ShowKey(FString prefix, int index){
        FString message = prefix;
        message += TEXT("CollectedPropertiesKey:");
        message += FString::Printf(TEXT("key %d: "), index);

        

        if(index >= 0 && index < keys.Num()){
            message += keys[index];
        }

        UE_LOG(LogTemp, Log, TEXT("%s"), *message);
    }
    



};

    