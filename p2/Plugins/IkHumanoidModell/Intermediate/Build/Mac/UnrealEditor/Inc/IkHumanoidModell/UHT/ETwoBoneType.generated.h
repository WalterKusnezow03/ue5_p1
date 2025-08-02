// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Ik/Bone/ETwoBoneType.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef IKHUMANOIDMODELL_ETwoBoneType_generated_h
#error "ETwoBoneType.generated.h already included, missing '#pragma once' in ETwoBoneType.h"
#endif
#define IKHUMANOIDMODELL_ETwoBoneType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Bone_ETwoBoneType_h


#define FOREACH_ENUM_ETWOBONETYPE(op) \
	op(ETwoBoneType::ETwoBoneLeg) \
	op(ETwoBoneType::ETwoBoneArm) 

enum class ETwoBoneType;
template<> struct TIsUEnumClass<ETwoBoneType> { enum { Value = true }; };
template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<ETwoBoneType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
