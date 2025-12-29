#include "P2AssetLoader.h"

#include "AssetPlugin/gamestart/AssetLoader.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"


#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/throwableEnum.h"
#include "AssetEnumCollection/assetEnums/particleEnum.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "AssetEnumCollection/assetEnums/entityEnum.h"


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
    loadWeaponAttachments();
    loadThrowables();
    loadParticles();
    loadTextures();
    
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



/// @brief load particles for the entitymanager
/// @param entityManager to set in
void P2AssetLoader::loadParticles(){
    
    AssetLoader::LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::smoke_enum, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/particle",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleSmoke"                   // Just the file name as displayed
    );

    AssetLoader::LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::fire_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/particle",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleFire"           // Just the file name as displayed
    );

    AssetLoader::LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::particleNone_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/particle",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleNone"           // Just the file name as displayed
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


