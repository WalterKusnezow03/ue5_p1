// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "gamestart/assetEnums/staticMeshEnum.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ASSETPLUGIN_staticMeshEnum_generated_h
#error "staticMeshEnum.generated.h already included, missing '#pragma once' in staticMeshEnum.h"
#endif
#define ASSETPLUGIN_staticMeshEnum_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_staticMeshEnum_h


#define FOREACH_ENUM_STATICMESHENUM(op) \
	op(staticMeshEnum::grassStaticMesh) 

enum class staticMeshEnum;
template<> struct TIsUEnumClass<staticMeshEnum> { enum { Value = true }; };
template<> ASSETPLUGIN_API UEnum* StaticEnum<staticMeshEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
