// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader.h"

#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"




AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
}


//constructor
void AssetLoader::loadAssets()
{

    loadMaterials(); //materials need to be loaded first to not have any issues
    loadEntities();

    loadWeapons();
    loadWeaponAttachments();
    loadThrowables();
    loadParticles();
    
    loadTextures();
    

    loadDebugCube();
}



/// @brief Method to load a Blueprint class from a path.
/// @param path The path to the Blueprint class.
/// @return The loaded UClass, or nullptr if it fails.
/// works as expected
UClass* AssetLoader::loadUClassBluePrint(FString path){
    // Load the class object dynamically
    UClass* bpClass = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
    
    
    // Check if the class was loaded successfully
    if (bpClass != nullptr)
    {
        return bpClass; //issues can still occur here when a non blueprint is found but treated like one!
    }
    return nullptr;
}

UMaterial *AssetLoader::loadMaterial(FString path){
    // Load the class object dynamically
    UMaterial* bpObject = LoadObject<UMaterial>(nullptr, *path);
    
    // Check if the class was loaded successfully
    if (bpObject != nullptr)
    {
        return bpObject;
    }
    return nullptr;
}




/// @brief passin your path and prefab blueprint name
/// @param path path like "/Game/Prefabs..."
/// @param bluePrintName "justTheName" of the file
/// @return FString path to load from
FString AssetLoader::buildPath(FString path, FString bluePrintName){
    FString pathFront = FString::Printf(TEXT("Blueprint'"));
    //FString room = FString::Printf(TEXT("room"));
    FString dot = FString::Printf(TEXT("."));
    FString _C_end = FString::Printf(TEXT("_C'"));
    
    FString connect = pathFront;
    connect.Append(path);
    connect.Append(bluePrintName);
    connect.Append(dot);
    connect.Append(bluePrintName);
    connect.Append(_C_end);

    cleanUpPath(connect);

    return connect;
}


void AssetLoader::cleanUpPath(FString &s){
    FString out;
    out.Reserve(s.Len());
    TCHAR slash = TEXT('/');
    TArray<TCHAR> asArray = s.GetCharArray();

    bool found = false;
    int i = 0;
    while(i  < asArray.Num()){
        if(asArray[i] != slash){
            out.AppendChar(asArray[i]);
            found = false;
        }
        else
        {
            if(!found){ //skip any else
                out.AppendChar(slash);
            }
            found = true;
        }
        i++;
    }
    s = out;
    //return out;
}

/// @brief load all entities 
void AssetLoader::loadEntities(){

    LoadAndSaveAssetToManager<entityEnum, UClass>(
        entityEnum::entity_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/player",        // like: "Prefabs/Weapons/attachments", no trailing slash
        "entityPrefab"           // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<entityEnum, UClass>(
        entityEnum::human_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/player",        // like: "Prefabs/Weapons/attachments", no trailing slash
        "humanEntityPrefab"           // Just the file name as displayed
    );



    /*
    if(assetManager *am = assetManager::instance()){
        FString path = FString::Printf(TEXT("/Game/Prefabs/player/"));
        FString entityString = buildPath(path, "entityPrefab");
        am->addBp(entityEnum::entity_enum, loadUClassBluePrint(entityString));


        FString humanString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/humanEntityPrefab.humanEntityPrefab_C'"));
        am->addBp(entityEnum::human_enum, loadUClassBluePrint(humanString));
    
    }*/


}








void AssetLoader::loadWeaponAttachments(){ 
    /*
    static void LoadAndSaveAssetToManager(
		E0 key0, //track in asset manager
		E1 key1, //track in asset manager
		FString pluginName, // like "Game" for game or any other plugin name
		FString innerPath, // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
		FString assetName //Just the file name as displayed
	)
    */

    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,      // track in asset manager
        weaponAttachmentEnum::reddot,  // track in asset manager
        "Game",                        // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "reddotBp"                     // Just the file name as displayed
    );
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,         // track in asset manager
        weaponAttachmentEnum::iron_sight, // track in asset manager
        "Game",                           // like "Game" for game or any other plugin name
        "Prefabs/Weapons/rifle2",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "ironsightbp"                     // Just the file name as displayed
    );
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,            // track in asset manager
        weaponAttachmentEnum::reddot,  // track in asset manager
        "Game",                        // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "reddotPistolBp"               // Just the file name as displayed
    );


    //supressor 2 times...
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,                           // track in asset manager
        weaponAttachmentEnum::muzzle_SoundSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "surpressorBp"                                // Just the file name as displayed
    );


    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,                           // track in asset manager
        weaponAttachmentEnum::muzzle_SoundSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "surpressorBp"                                // Just the file name as displayed
    );

    //muendungs feuer daempfer 2 times

    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::pistol,                           // track in asset manager
        weaponAttachmentEnum::muzzle_flashSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "MundungsfeuerdampferBp"                      // Just the file name as displayed
    );
    LoadAndSaveAssetToManager<weaponEnum, weaponAttachmentEnum, UClass>(
        weaponEnum::assaultRifle,                           // track in asset manager
        weaponAttachmentEnum::muzzle_flashSurpressor, // track in asset manager
        "Game",                                       // like "Game" for game or any other plugin name
        "Prefabs/Weapons/attachments",                // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "MundungsfeuerdampferBp"                      // Just the file name as displayed
    );


    /*
    if(assetManager *a = assetManager::instance()){
        AssetPathMaker pathMaker;

        
        FString reddotString = pathMaker.makeAssetPath(
            EAssetType::EUClassBlueprint,
            "Game",
            "Prefabs/Weapons/attachments",
            "reddotBp"
        );
        UClass *bp = loadUClassBluePrint(reddotString);
        a->addBp(weaponEnum::assaultRifle, weaponAttachmentEnum::reddot, bp);
    
        
        
        FString ironsightString = pathMaker.makeAssetPath(
            EAssetType::EUClassBlueprint,
            "Game",
            "Prefabs/Weapons/rifle2",
            "ironsightbp");
        UClass *bp1 = loadUClassBluePrint(ironsightString);
        a->addBp(weaponEnum::assaultRifle, weaponAttachmentEnum::iron_sight, bp1);


        
        FString reddotStringPistol = FString::Printf(TEXT(
            "Blueprint'/Game/Prefabs/Weapons/attachments/reddotPistolBp.reddotPistolBp_C'"
        ));
        UClass *bp2 = loadUClassBluePrint(reddotStringPistol);
        a->addBp(weaponEnum::pistol, weaponAttachmentEnum::reddot, bp2);




        FString surpressorString = FString::Printf(TEXT(
            "Blueprint'/Game/Prefabs/Weapons/attachments/surpressorBp.surpressorBp_C'"
        ));
        UClass *bp3 = loadUClassBluePrint(surpressorString);
        a->addBp(weaponEnum::pistol, weaponAttachmentEnum::muzzle_SoundSurpressor, bp3);
        a->addBp(weaponEnum::assaultRifle, weaponAttachmentEnum::muzzle_SoundSurpressor, bp3);

        FString muzzleflashSurpressorString = FString::Printf(TEXT(
            "Blueprint'/Game/Prefabs/Weapons/attachments/MundungsfeuerdampferBp.MundungsfeuerdampferBp_C'"
        ));
        UClass *bp4 = loadUClassBluePrint(muzzleflashSurpressorString);
        a->addBp(weaponEnum::pistol, weaponAttachmentEnum::muzzle_flashSurpressor, bp4);
        a->addBp(weaponEnum::assaultRifle, weaponAttachmentEnum::muzzle_flashSurpressor, bp4);


        
    }*/
}




/// @brief loads all weapons to the entity manager
void AssetLoader::loadWeapons(){

    LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::pistol,                 // track in asset manager
        "Game",                             // like "Game" for game or any other plugin name
        "Prefabs/Weapons/pistol/pistolNew", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "pistolNew"                         // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::assaultRifle,                 // track in asset manager
        "Game",                             // like "Game" for game or any other plugin name
        "Prefabs/Weapons/rifle2", // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "rifleBp"                         // Just the file name as displayed
    );


    LoadAndSaveAssetToManager<weaponEnum, UClass>(
        weaponEnum::thrower,            // track in asset manager
        "Game",                         // like "Game" for game or any other plugin name
        "Prefabs/Throwables",           // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "defaultthrower"                // Just the file name as displayed
    );
    


    /*
    if(assetManager *a = assetManager::instance()){
        
        //pistol
        FString pistolString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/Weapons/pistol/pistolNew/pistolNew.pistolNew_C'"));
        UClass *bp = loadUClassBluePrint(pistolString);
        a->addBp(weaponEnum::pistol, bp);

        
        //ar
        FString rifleString = FString::Printf(
            //TEXT("Blueprint'/Game/Prefabs/Weapons/rifle/rifleBp.rifleBp_C'")
            TEXT("Blueprint'/Game/Prefabs/Weapons/rifle2/rifleBp.rifleBp_C'")
        );
        UClass *riflebp = loadUClassBluePrint(rifleString);
        a->addBp(weaponEnum::assaultRifle, riflebp);

        //thrower
        UClass *throwerBp = loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/defaultthrower.defaultthrower_C'"));
        a->addBp(weaponEnum::thrower, throwerBp);

    }*/

}


/// @brief load throwables
/// @param entityManager entity manager
void AssetLoader::loadThrowables(){
    
    LoadAndSaveAssetToManager<throwableEnum, UClass>(
        throwableEnum::greneade_enum, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/Throwables",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "grenadeBp"                   // Just the file name as displayed
    );
    
    /*
    if(assetManager *a = assetManager::instance()){

        a->addBp(
            throwableEnum::greneade_enum,
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/grenadeBp.grenadeBp_C'"))
        );
    }*/
    
}



/// @brief load particles for the entitymanager
/// @param entityManager to set in
void AssetLoader::loadParticles(){
    
    LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::smoke_enum, // track in asset manager
        "Game",                       // like "Game" for game or any other plugin name
        "Prefabs/particle",         // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleSmoke"                   // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::fire_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/particle",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleFire"           // Just the file name as displayed
    );

    LoadAndSaveAssetToManager<particleEnum, UClass>(
        particleEnum::particleNone_enum, // track in asset manager
        "Game",                  // like "Game" for game or any other plugin name
        "Prefabs/particle",      // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
        "particleNone"           // Just the file name as displayed
    );
    

    /*
    if(assetManager *am = assetManager::instance()){

        am->addBp(
            particleEnum::smoke_enum, 
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/particle/particleSmoke.particleSmoke_C'"))
        );

        am->addBp(
            particleEnum::fire_enum, 
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/particle/particleFire.particleFire_C'"))
        );

        am->addBp(
            particleEnum::particleNone_enum, 
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/particle/particleNone.particleNone_C'"))
        );
    }*/


}



// ---- LOAD MATERIALS ----

/// @brief loads all materials needed
void AssetLoader::loadMaterials(){
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
    assetNames[materialEnum::sandMaterial] = "sandMaterial";
    assetNames[materialEnum::redsandMaterial] = "redsandMaterial";
    assetNames[materialEnum::waterMaterial] = "waterMaterial";

    assetNames[materialEnum::snowMaterial] = "snowMaterial";
    assetNames[materialEnum::_texturedMaterial] = "texturedMaterial";
    assetNames[materialEnum::prop_alarmBoxMaterial] = "prop_alarmBoxMaterial";
    assetNames[materialEnum::wallMaterial] = "wallMaterial";
    assetNames[materialEnum::glassMaterial] = "glassMaterial";

    for(auto &pair : assetNames){
        materialEnum typeEnum = pair.first;
        FString nameAsset = pair.second;
        LoadAndSaveAssetToManager<materialEnum, UMaterial>(
            typeEnum, 
            "Game", //plugin name or "Game" 
            "Prefabs/terrain/materials", //inner path, not trailing slash
            nameAsset //asset name just as displayed
        );
    }

    /*
    if(assetManager *a = assetManager::instance()){

        //TERRAIN MATERIALS
        //grass material
        a->addMaterial(
            materialEnum::grassMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/grassMaterial.grassMaterial'"))
        );

        //stone material
        a->addMaterial(
            materialEnum::stoneMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/stoneMaterial.stoneMaterial'"))
        );
        a->addMaterial(
            materialEnum::beigeStoneMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/beigeStoneMaterial.beigeStoneMaterial'"))
        );

        //wing
        a->addMaterial(
            materialEnum::wingMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/wingMaterial.wingMaterial'"))
        );




        //tree material
        a->addMaterial(
            materialEnum::treeMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/treeMaterial.treeMaterial'"))
        );
        a->addMaterial(
            materialEnum::palmLeafMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/palmLeafMaterial.palmLeafMaterial'"))
        );

        //sand material
        a->addMaterial(
            materialEnum::sandMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/sandMaterial.sandMaterial'"))
        );
        a->addMaterial(
            materialEnum::redsandMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/redsandMaterial.redsandMaterial'"))
        );
        
        //water material
        a->addMaterial(
            materialEnum::waterMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/waterMaterial.waterMaterial'"))
        );

        //snow material
        a->addMaterial(
            materialEnum::snowMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/snowMaterial.snowMaterial'"))
        );

        //textured material
        a->addMaterial(
            materialEnum::_texturedMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/texturedMaterial.texturedMaterial'"))
        );

        a->addMaterial(
            materialEnum::prop_alarmBoxMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/terrain/materials/prop_alarmBoxMaterial.prop_alarmBoxMaterial'"))
        );

        //HOUSE MATERIALS
        //wall material
        a->addMaterial(
            materialEnum::wallMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/rooms/materials/wallMaterial.wallMaterial'"))
        );

        //glass material
        a->addMaterial(
            materialEnum::glassMaterial,
            loadMaterial(TEXT("Material'/Game/Prefabs/rooms/materials/glassMaterial.glassMaterial'"))
        );

        

    }*/

}






void AssetLoader::loadTextures(){
    
    //icons directory
    std::map<textureEnum, FString> assetNames;
    assetNames[textureEnum::patroneIcon] = "patroneIcon";
    assetNames[textureEnum::healthIcon] = "healthIcon";
    assetNames[textureEnum::enemyMarkerIcon] = "enemyMarkerIcon";
    assetNames[textureEnum::playerMarkerIcon] = "playerMarkerIcon";

    for(auto &pair : assetNames){
        textureEnum typeEnum = pair.first;
        FString nameAsset = pair.second;
        LoadAndSaveAssetToManager<textureEnum, UTexture2D>(
            typeEnum, 
            "Game", //plugin name or "Game" 
            "Prefabs/icons", //inner path, not trailing slash
            nameAsset //asset name just as displayed
        );
    }
    
    
    
    
    
    
    
    /*
    if(assetManager *am = assetManager::instance()){

        am->addTexture(
            textureEnum::patroneIcon,
            AssetLoader::loadAsset<UTexture2D>(TEXT(
                "Texture2D'/Game/Prefabs/icons/patroneIcon.patroneIcon'")
            )
        );

        am->addTexture(
            textureEnum::healthIcon,
            AssetLoader::loadAsset<UTexture2D>(TEXT(
                "Texture2D'/Game/Prefabs/icons/healthIcon.healthIcon'")
            )
        );

        am->addTexture(
            textureEnum::enemyMarkerIcon,
            AssetLoader::loadAsset<UTexture2D>(TEXT(
                "Texture2D'/Game/Prefabs/icons/enemyMarkerIcon.enemyMarkerIcon'")
            )
        );

        am->addTexture(
            textureEnum::playerMarkerIcon,
            AssetLoader::loadAsset<UTexture2D>(TEXT(
                "Texture2D'/Game/Prefabs/icons/playerMarkerIcon.playerMarkerIcon'")
            )
        );

    }*/
}





void AssetLoader::loadDebugCube(){
    if(assetManager *am = assetManager::instance()){

        FString path = FString::Printf(TEXT(
            "Blueprint'/Game/Prefabs/debug/debugCube.debugCube_C'"
        ));
        am->setDebugCubeBp(loadUClassBluePrint(path));
    }
}


