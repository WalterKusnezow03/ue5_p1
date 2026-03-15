#include "P2AssetLoader.h"

#include "AssetPlugin/gamestart/AssetLoader.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"


#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/throwableEnum.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "AssetEnumCollection/assetEnums/entityEnum.h"
#include "AssetEnumCollection/assetEnums/Eui3DWidgetEnum.h"
#include "p2/ui/3Dui/GamePlayWidgets/Enum/EWorldDynamicWidgetEnum.h"
#include "AssetEnumCollection/assetEnums/EGameActorEnum.h"
#include "AssetEnumCollection/assetEnums/textureEnum.h"
#include "AssetEnumCollection/assetEnums/EDataAssetEnum.h"
#include "p2/weapon/animationEnum/EweaponAnimation.h"
#include <map>

#include "p2/vfx/base/VFXActor.h"
#include "p2/vfx/type/EVFXActorType.h"


/*
template <typename E0, typename E1, typename T>
static void LoadAndSaveAssetToManager(
    E0 key0, //track in asset manager
    E1 key1, //track in asset manager
    FString pluginName, // like "Game" for game or any other plugin name
    FString innerPath, // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
    FString assetName //Just the file name as displayed
)
*/



//constructor
void P2AssetLoader::LoadGameAssets(){

    loadMaterials(); //materials need to be loaded first to not have any issues
    loadEntities();
    loadWeapons();
    loadWeaponAnimations();
    loadWeaponAttachments();
    loadThrowables();
    loadVFX();
    loadTextures();

    Load3DUiAssets();
}


/// @brief load all entities 
void P2AssetLoader::loadEntities(){

    AssetLoader::LoadAndSaveAssetToManager<entityEnum, UClass>(
        entityEnum::entity_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/player",        // like: "Prefabs/Weapons/attachments", no trailing slash
        "entityPrefab"           // Just the file name as displayed
    );

    AssetLoader::LoadAndSaveAssetToManager<entityEnum, UClass>(
        entityEnum::human_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/player",        // like: "Prefabs/Weapons/attachments", no trailing slash
        "humanEntityPrefab"           // Just the file name as displayed
    );

}








void P2AssetLoader::loadWeaponAttachments(){ 
    /*
    static void LoadAndSaveAssetToManager(
		E0 key0, //track in asset manager
		E1 key1, //track in asset manager
		FString pluginName, // like "Game" for game or any other plugin name
		FString innerPath, // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
		FString assetName //Just the file name as displayed
	)
    */

    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,      // track in asset manager
        weaponAttachmentEnum::reddot,  // track in asset manager
        "Game",                        // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "reddotBp"                     // Just the file name as displayed
    );
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,         // track in asset manager
        weaponAttachmentEnum::iron_sight, // track in asset manager
        "Game",                           // like "Game" for game or any other plugin name
        "Prefabs/Weapons/rifle2",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "ironsightbp"                     // Just the file name as displayed
    );
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,            // track in asset manager
        weaponAttachmentEnum::reddot,  // track in asset manager
        "Game",                        // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "reddotPistolBp"               // Just the file name as displayed
    );


    //supressor 2 times...
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,                           // track in asset manager
        weaponAttachmentEnum::muzzle_SoundSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "surpressorBp"                                // Just the file name as displayed
    );


    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,                           // track in asset manager
        weaponAttachmentEnum::muzzle_SoundSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "surpressorBp"                                // Just the file name as displayed
    );

    //muendungs feuer daempfer 2 times

    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,                           // track in asset manager
        weaponAttachmentEnum::muzzle_flashSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "MundungsfeuerdampferBp"                      // Just the file name as displayed
    );
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,                           // track in asset manager
        weaponAttachmentEnum::muzzle_flashSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "MundungsfeuerdampferBp"                      // Just the file name as displayed
    );


}




/// @brief loads all weapons to the entity manager
void P2AssetLoader::loadWeapons(){

    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::pistol,                 // track in asset manager
        "Game",                             // like "Game" for game or any other plugin name
        "Prefabs/Weapons/pistol/pistolNew", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "pistolNew"                         // Just the file name as displayed
    );

    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::assaultRifle,                 // track in asset manager
        "Game",                             // like "Game" for game or any other plugin name
        "Prefabs/Weapons/rifle2", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "rifleBp"                         // Just the file name as displayed
    );


    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::thrower,            // track in asset manager
        "Game",                         // like "Game" for game or any other plugin name
        "Prefabs/Throwables",           // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "defaultthrower"                // Just the file name as displayed
    );

    //
    //Users/walterk/Documents/UnrealProjects/p2/Content/Prefabs/Weapons/c4/c4BP.uasset
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::C4,       // track in asset manager
        "Game",               // like "Game" for game or any other plugin name
        "Prefabs/Weapons/c4", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "c4BP"                // Just the file name as displayed
    );


    
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::grenade,       // track in asset manager
        "Game", // like "Game" for game or any other plugin name
        "Prefabs/Weapons/grenade", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "throwableGrenadeBP"                // Just the file name as displayed
    );
}

void P2AssetLoader::loadWeaponAnimations(){
    
    //UAnimSequence
    //pistol loading
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, EweaponAnimation, UAnimSequence>(
        weaponEnum::pistol,
        EweaponAnimation::verschlussAnimationShoot,
        "Game",               
        "Prefabs/Weapons/pistol/pistolNew", 
        "verschlussAnim"// Just the file name as displayed
    );
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, EweaponAnimation, UAnimSequence>(
        weaponEnum::pistol,
        EweaponAnimation::magAnimationReload,
        "Game",               
        "Prefabs/Weapons/pistol/pistolMag", 
        "magAnim"// Just the file name as displayed
    );

    //ar loading
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, EweaponAnimation, UAnimSequence>(
        weaponEnum::assaultRifle,
        EweaponAnimation::verschlussAnimationShoot,
        "Game",               
        "Prefabs/weapons/rifle2", 
        "verschlussAnim"// Just the file name as displayed
    );
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, EweaponAnimation, UAnimSequence>(
        weaponEnum::assaultRifle,
        EweaponAnimation::magAnimationReload,
        "Game",               
        "Prefabs/Weapons/rifle2", 
        "magAnim"// Just the file name as displayed
    );

    //note: never used: custom kick back lerp.
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, EweaponAnimation, UAnimSequence>(
        weaponEnum::assaultRifle,
        EweaponAnimation::gehauseAnimationShoot,
        "Game",               
        "Prefabs/weapons/rifle2", 
        "gehauseAnim"// Just the file name as displayed
    );
    AssetLoader::LoadAndSaveAssetToManager<weaponEnum, EweaponAnimation, UAnimSequence>(
        weaponEnum::assaultRifle,
        EweaponAnimation::magAnimationShoot,
        "Game",               
        "Prefabs/Weapons/rifle2", 
        "magAnimShoot"// Just the file name as displayed
    );


}








/// @brief load throwables
/// @param entityManager entity manager
void P2AssetLoader::loadThrowables(){
    
    AssetLoader::LoadAndSaveAssetToManager<throwableEnum, UClass>(
        throwableEnum::greneade_enum, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/Throwables",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "grenadeBp"                   // Just the file name as displayed
    );
    
    
}




//NEW
void P2AssetLoader::loadVFX(){

    AssetLoader::LoadAndSaveAssetToManager<EVFXActorType, UClass>(
        EVFXActorType::EExplosion, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/particle/actorBP",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "ExplosionActorBP"                   // Just the file name as displayed
    );




}








// ---- LOAD MATERIALS ----

/// @brief loads all materials needed
void P2AssetLoader::loadMaterials(){
    /*
    Default call:

    LoadAndSaveAssetToManager<materialEnum, UMaterial>(
        materialEnum::grassMaterial, 
        "Game", //plugin name or "Game" 
        "Prefabs/terrain/materials", //inner path, not trailing slash
        "grassMaterial" //asset name just as displayed
    );*/

    //CAUTION:
    //since im saving all materials in the same folder:
    std::map<materialEnum, FString> assetNames;
    assetNames[materialEnum::grassMaterial] = "grassMaterial";
    assetNames[materialEnum::stoneMaterial] = "stoneMaterial";
    assetNames[materialEnum::beigeStoneMaterial] = "beigeStoneMaterial";
    assetNames[materialEnum::wingMaterial] = "wingMaterial";
    assetNames[materialEnum::treeMaterial] = "treeMaterial";
    assetNames[materialEnum::palmLeafMaterial] = "palmLeafMaterial";
    assetNames[materialEnum::CactusMaterial] = "CactusMaterial";
    assetNames[materialEnum::sandMaterial] = "sandMaterial";
    assetNames[materialEnum::redsandMaterial] = "redsandMaterial";
    assetNames[materialEnum::waterMaterial] = "waterMaterial";

    assetNames[materialEnum::snowMaterial] = "snowMaterial";
    assetNames[materialEnum::_texturedMaterial] = "texturedMaterial";
    assetNames[materialEnum::prop_alarmBoxMaterial] = "prop_alarmBoxMaterial";
    assetNames[materialEnum::wallMaterial] = "wallMaterial";
    assetNames[materialEnum::glassMaterial] = "glassMaterial";
    assetNames[materialEnum::grassMaterialWithShader] = "grassMaterialWithShader";
    

    for(auto &pair : assetNames){
        materialEnum typeEnum = pair.first;
        FString nameAsset = pair.second;
        AssetLoader::LoadAndSaveAssetToManager<materialEnum, UMaterial>(
            typeEnum, 
            "Game", //plugin name or "Game" 
            "Prefabs/terrain/materials", //inner path, not trailing slash
            nameAsset //asset name just as displayed
        );
    }


}






void P2AssetLoader::loadTextures(){
    
    //icons directory
    std::map<textureEnum, FString> assetNames;
    assetNames[textureEnum::patroneIcon] = "patroneIcon";
    assetNames[textureEnum::healthIcon] = "healthIcon";
    assetNames[textureEnum::enemyMarkerIcon] = "enemyMarkerIcon";
    assetNames[textureEnum::playerMarkerIcon] = "playerMarkerIcon";

    for(auto &pair : assetNames){
        textureEnum typeEnum = pair.first;
        FString nameAsset = pair.second;
        AssetLoader::LoadAndSaveAssetToManager<textureEnum, UTexture2D>(
            typeEnum, 
            "Game", //plugin name or "Game" 
            "Prefabs/icons", //inner path, not trailing slash
            nameAsset //asset name just as displayed
        );
    }
    
}






void P2AssetLoader::Load3DUiAssets(){
    loadUiDataAssets();
    loadLoadoutRoomAssets();
    loadGameStartRoomAssets();
    loadPauseRoomAssets();
    load3DWidgetData();
}

void P2AssetLoader::loadUiDataAssets(){
    //EDataAssetEnum::EUiColorAsset

    AssetLoader::LoadAndSavePrimaryDataAssetToAssetToManager<EDataAssetEnum, UPrimaryDataAsset>(
        EDataAssetEnum::EUiColorAsset, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/ui3D/DataAsset/",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "ColorAssetBP"                   // Just the file name as displayed
    );
}
    



void P2AssetLoader::loadLoadoutRoomAssets(){
    AssetLoader::LoadAndSaveAssetToManager<EGameActorEnum, UClass>(
        EGameActorEnum::ELoadoutRoom, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/ui3D/RoomActors/Loadout",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "Loadoutroombp"                   // Just the file name as displayed
    );
}
void P2AssetLoader::loadGameStartRoomAssets(){
    AssetLoader::LoadAndSaveAssetToManager<EGameActorEnum, UClass>(
        EGameActorEnum::EGameStartRoom, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/ui3D/RoomActors/Gamestart",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "Gamestartroombp"                   // Just the file name as displayed
    );
}



void P2AssetLoader::loadPauseRoomAssets(){
    AssetLoader::LoadAndSaveAssetToManager<EGameActorEnum, UClass>(
        EGameActorEnum::EPauseRoom, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/ui3D/RoomActors/Pause",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "Pauseroomactorbp"                   // Just the file name as displayed
    );
}



void P2AssetLoader::load3DWidgetData(){
    loadWidgetMaterial();
    load3DWidgets();
}

//might not be needed (removing the material.)
void P2AssetLoader::loadWidgetMaterial(){
    AssetLoader::LoadAndSaveAssetToManager<materialEnum, UMaterial>(
        materialEnum::widgetMaterial, 
        "Game", //plugin name or "Game" 
        "Prefabs/ui3D", //inner path, not trailing slash
        "widgetMaterial" //asset name just as displayed
    );
}

void P2AssetLoader::load3DWidgets(){

   

    
    AssetLoader::LoadAndSaveAssetToManager<Eui3DWidgetEnum, UClass>(
        Eui3DWidgetEnum::EPlayerHud, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/ui3D",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "hudUi3DBP"           // Just the file name as displayed
    );



    AssetLoader::LoadAndSaveAssetToManager<Eui3DWidgetEnum, UClass>(
        Eui3DWidgetEnum::EWeaponTableWidget, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/ui3D/weapon3Dui",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "WeaponTableUiActor"           // Just the file name as displayed
    );



    //EWorldDynamicWidgetEnum on purpose!
    AssetLoader::LoadAndSaveAssetToManager<EWorldDynamicWidgetEnum, UClass>(
        EWorldDynamicWidgetEnum::EInteractWidget, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/ui3D/gameWorldUi/InteractWidget",// like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "InteractWidgetActorBP"           // Just the file name as displayed
    );

    

}


