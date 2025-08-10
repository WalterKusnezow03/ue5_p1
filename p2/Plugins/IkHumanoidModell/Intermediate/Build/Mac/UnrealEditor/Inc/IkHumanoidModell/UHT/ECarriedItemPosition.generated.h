// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "carryItems/enum/ECarriedItemPosition.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef IKHUMANOIDMODELL_ECarriedItemPosition_generated_h
#error "ECarriedItemPosition.generated.h already included, missing '#pragma once' in ECarriedItemPosition.h"
#endif
#define IKHUMANOIDMODELL_ECarriedItemPosition_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_enum_ECarriedItemPosition_h


#define FOREACH_ENUM_ECARRIEDITEMPOSITION(op) \
	op(ECarriedItemPosition::EHipPosition) \
	op(ECarriedItemPosition::ETorsoPosition) \
	op(ECarriedItemPosition::EAimDownSightPosition) 

enum class ECarriedItemPosition;
template<> struct TIsUEnumClass<ECarriedItemPosition> { enum { Value = true }; };
template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<ECarriedItemPosition>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
