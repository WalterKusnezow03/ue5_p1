// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetPlugin/gamestart/TAssetManager/assetManagerGeneric.h"
#include "AssetPlugin/gamestart/TAssetManager/AssetManagerTwoGeneric.h"

#include "AssetPlugin/gamestart/assetEnums/entityEnum.h"
#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "AssetPlugin/gamestart/assetEnums/weaponEnum.h"
#include "AssetPlugin/gamestart/assetEnums/particleEnum.h"
#include "AssetPlugin/gamestart/assetEnums/throwableEnum.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"

#include "AssetPlugin/gamestart/ExternalEnumTracking/NamedEnumBase.h"
#include "AssetPlugin/gamestart/ExternalEnumTracking/NamedEnum.h"

#include "AssetPlugin/gamestart/PathMaker/enum/EAssetType.h"
#include <map>

/**
 * 
 */
class ASSETPLUGIN_API assetManager
{
private:
	bool useFakeRTTI = true;

public:
	static void EndGame();
	static assetManager *instance();
	~assetManager();


	//entity
	UClass *findBp(entityEnum type);
	void addBp(entityEnum type, UClass *asset);


	//weapon
	UClass *findBp(weaponEnum type);
	void addBp(weaponEnum type, UClass *asset);

	//throwables
	UClass *findBp(throwableEnum type);
	void addBp(throwableEnum type, UClass *asset);

	//particles
	UClass *findBp(particleEnum type);
	void addBp(particleEnum type, UClass *asset);


	//all materials
	UMaterialInterface *findMaterial(materialEnum type);
	void addMaterial(materialEnum type, UMaterialInterface *material);

	//weapon attachments
	UClass *findBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment);
	void addBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment, UClass *uclass);


	//ui

	UTexture2D *findTexture(textureEnum type);
	void addTexture(textureEnum type, UTexture2D *texture);




	void setDebugCubeBp(UClass *uclassin);
	UClass *debugCubeBp();




	//---- FAKE Runtime type info for dynamic enum tracking SECTION ----
	//(Works fine.)

	/// @brief E must be Unreal UENUM!, tracks the enum value, enter a name which wasnt used before.
	template<typename E>
	void Track(FString outername){
		if(TrackedMap.find(outername) != TrackedMap.end()){
			//name is already in use!
			return;
		}

		FString enumName = EnumName<E>();
		if(enumName != TEXT("invalidEnum")){
			//actually new enum
			if(TrackedEnums.find(enumName) == TrackedEnums.end()){
				FNamedEnum<E> *NamedEnum = new FNamedEnum<E>(outername);
				TrackedEnums[enumName] = NamedEnum;

				if(bLogEnabled){
					FString prefix = FString::Printf(TEXT("assetManager Track<E> Tracked : %s"), *outername);
					UE_LOG(LogTemp, Log, TEXT("%s"), *prefix);
				}
				return;
			}
		}else{
			if(bLogEnabled){
				FString prefix = FString::Printf(TEXT("assetManager Track<E>: Name invalid! %s"), *outername);
    			UE_LOG(LogTemp, Log, TEXT("%s"), *prefix);
			}
		}
		// already tracked
	}


	template<typename E, typename T> 
	void Add(E e, T *ptr){
		static_assert(std::is_base_of<UObject, T>::value, "must be an UObject");

		FString outerKey;
		FString innerKey;
		if(MakeKeyPair(e, outerKey, innerKey)){
			if(TrackedMap.find(outerKey) == TrackedMap.end()){
				TrackedMap[outerKey] = new assetManagerGeneric<FString, UObject>();
			}
			assetManagerGeneric<FString, UObject> *manager = TrackedMap[outerKey];
			if(manager){
				ptr->AddToRoot(); //Add to root here, removed in generic map later, inside.

				manager->addRaw(innerKey, ptr);
			}
		}
	}

	template<typename E, typename T> 
	T* Find(E e){
		static_assert(std::is_base_of<UObject, T>::value, "must be an UObject");
		FString outerKey;
		FString innerKey;
		if(MakeKeyPair(e, outerKey, innerKey)){
			if(TrackedMap.find(outerKey) != TrackedMap.end()){
				assetManagerGeneric<FString, UObject> *manager = TrackedMap[outerKey];
				if(manager){
					UObject *ptr = manager->getBp(innerKey);
					if(ptr){
						T *casted = Cast<T>(ptr);
						if(casted){
							return casted;
						}
					}
				}
			}
		}
		return nullptr;
	}

private:

	/// @brief makes the outer and inner key to find the assetmanager and internal enum value
	/// but both as string
	template<typename E> 
	bool MakeKeyPair(E e, FString &outerKey, FString &innerKey){
	
		FString enumAsString = EnumName<E>();
		if (TrackedEnums.find(enumAsString) != TrackedEnums.end())
		{
			FNamedEnumBase *NamedEnum = TrackedEnums[enumAsString];
			if(NamedEnum){
				FNamedEnum<E> *casted = static_cast<FNamedEnum<E> *>(NamedEnum);
				if(casted && casted->Contains(e)){
					outerKey = casted->GetName();
					innerKey = casted->makeKey(e);

					if(bLogEnabled){
						FString made = FString::Printf(TEXT("assetManager Tracked Key : %s %s"), *outerKey, *innerKey);
    					UE_LOG(LogTemp, Log, TEXT("%s"), *made);
					}
					return true;
				}
			}
		}
		return false;
	}

	///@brief enum name read from header definition.
	///used to store FNamedEnum Structs 
	template<typename E>
	FString EnumName(){
		UEnum* EnumPtr = StaticEnum<E>(); //isA UObject
		FString outname = TEXT("invalidEnum");
		if(EnumPtr){
			//is just as in the Header definition, the enum name.
			outname = EnumPtr->GetFName().ToString();

			if(bLogEnabled){
				FString message = FString::Printf(TEXT("assetManager Tracked Enum As String : %s"), *outname);
				UE_LOG(LogTemp, Log, TEXT("%s"), *message);
			}
			
		}
		return outname;
	}

private:
	assetManager();
	static class assetManager *instancePointer;


	//tracked FNamedEnum List
	std::map<FString, FNamedEnumBase *> TrackedEnums;
	std::map<FString, assetManagerGeneric<FString, UObject>*> TrackedMap;
	void setupDefaultTracker();
	void Clear();

	bool bLogEnabled = false;

	//---- FAKE RTTI SECTION END ----

	

	// --- DEPRECATED ---

	//all asset maps
	assetManagerGeneric<entityEnum, UClass> entityAssets;
	assetManagerGeneric<weaponEnum, UClass> weaponAssets;
	assetManagerGeneric<throwableEnum, UClass> throwableAssets;
	assetManagerGeneric<particleEnum, UClass> particleAssets;

	assetManagerGeneric<materialEnum, UMaterialInterface> materialAssets;
	assetManagerGeneric<textureEnum, UTexture2D> textureAssets;

	assetManagerTwoGeneric<weaponEnum, weaponAttachmentEnum, UClass> weaponAttachmentAssets;
	

	UClass *debugCubePointer = nullptr;
};
