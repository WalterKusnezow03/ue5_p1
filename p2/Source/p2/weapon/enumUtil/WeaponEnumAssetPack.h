#pragma once

#include "CoreMinimal.h"

#include "p2/weapon/enumUtil/WeaponPropertyAsset.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "WeaponEnumAssetPack.generated.h"



/// @brief will store a map of Weapon Enum Structs, saving string conversion,
/// damage, attachments and other data for quick and bundled conversion
UCLASS(BlueprintType)
class COREMATH_API UWeaponEnumAssetPack : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    
    void ValidatePropertiesOnInit();

    //weapon enum data map.
    UPROPERTY(EditAnywhere, Category="WeaponEnumAssetPacks")
    TMap<weaponEnum, FWeaponPropertyAsset> conversionAssets;

    

    // ---- attachment data ----
    UPROPERTY(EditAnywhere, Category="WeaponAttachmentData")
    TMap<weaponAttachmentEnum, FString> attachmentNames;

    UPROPERTY(EditAnywhere, Category="WeaponAttachmentData")
    TArray<weaponAttachmentEnum> sightAttachments;

    UPROPERTY(EditAnywhere, Category="WeaponAttachmentData")
    TArray<weaponAttachmentEnum> muzzleAttachments;

    UPROPERTY(EditAnywhere, Category="WeaponAttachmentData")
    TArray<weaponAttachmentEnum> gripAttachments;
    // ---- attachment data ----

    // -- load --
    //static void LoadAssetData();

    // -- functions -- 

    bool CanHaveAttachments(weaponEnum typeWeapon);
    bool isASightAttachment(weaponAttachmentEnum type);
    bool isAMuzzleAttachment(weaponAttachmentEnum type);
    bool isAGripAttachment(weaponAttachmentEnum type);

    FString toString(weaponEnum type);
    FString toString(weaponAttachmentEnum type);

    ///needed for explicit throwable weapon to ammunition conversion, not
    ///rifles / pistols! baseball bat, grenade, etc.! (1/1) map, not (1/n)!
    bool Convert(ammunitionEnum ammo, weaponEnum &typeout);
    bool Convert(weaponEnum weaponType, ammunitionEnum &typeout);
    bool IsThrowable(ammunitionEnum type);


    int getMagSize(weaponEnum weaponType);
    float recoilValue(weaponEnum weaponType);
    float reloadTime(weaponEnum weaponType);

    /// @brief damage over a distance by weapon type
    float damageFor(weaponEnum weaponType, FVector &a, FVector &b);
    float CooldownTimeBasedOnRpm(weaponEnum weaponType);
    ammunitionEnum getAmmunitionType(weaponEnum weaponType);
    bool isSingleFireOnly(weaponEnum weaponType);

    EMagSocketType getMagSocketType(weaponEnum type);
    

protected:
    FWeaponPropertyAsset *FindByAmmunition(ammunitionEnum ammoType);
    FWeaponPropertyAsset *FindByWeapon(weaponEnum type);

    template <typename E, typename T>
    bool HasType(E enumtype, TMap<E, T> &mapRef){
        if (T* value = mapRef.Find(enumtype)){
            return true;
        }
        return false;
    }

    template <typename E>
    bool HasType(E enumtype, TArray<E> &array){
        return array.Contains(enumtype);
    }
};