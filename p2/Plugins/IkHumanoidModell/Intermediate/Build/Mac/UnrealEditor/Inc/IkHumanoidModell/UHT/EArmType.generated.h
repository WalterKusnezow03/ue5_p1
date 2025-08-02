// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Ik/Controller/TorsoController/EArmType.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef IKHUMANOIDMODELL_EArmType_generated_h
#error "EArmType.generated.h already included, missing '#pragma once' in EArmType.h"
#endif
#define IKHUMANOIDMODELL_EArmType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_TorsoController_EArmType_h


#define FOREACH_ENUM_EARMTYPE(op) \
	op(EArmType::ELeft) \
	op(EArmType::ERight) 

enum class EArmType;
template<> struct TIsUEnumClass<EArmType> { enum { Value = true }; };
template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<EArmType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
