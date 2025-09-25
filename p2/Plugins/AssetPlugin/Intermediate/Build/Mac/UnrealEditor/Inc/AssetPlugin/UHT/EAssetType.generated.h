// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/PathMaker/enum/EAssetType.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_EAssetType_generated_h
#error "EAssetType.generated.h already included, missing '#pragma once' in EAssetType.h"
#endif
#define ASSETPLUGIN_EAssetType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_PathMaker_enum_EAssetType_h


#define FOREACH_ENUM_EASSETTYPE(op) \
	op(EAssetType::EUTexture2D) \
	op(EAssetType::EUClassBlueprint) \
	op(EAssetType::EUMaterial) \
	op(EAssetType::EUStaticMesh) 

enum class EAssetType;
template<> struct TIsUEnumClass<EAssetType> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<EAssetType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
