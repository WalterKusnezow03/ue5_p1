// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "actor/debugEnum/EDebugPart.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef IKHUMANOIDMODELL_EDebugPart_generated_h
#error "EDebugPart.generated.h already included, missing '#pragma once' in EDebugPart.h"
#endif
#define IKHUMANOIDMODELL_EDebugPart_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_debugEnum_EDebugPart_h


#define FOREACH_ENUM_EDEBUGPART(op) \
	op(EDebugPart::EDebugArm) \
	op(EDebugPart::EDebugLayeredArm) \
	op(EDebugPart::EDebugTorso) \
	op(EDebugPart::EDebugHipController) \
	op(EDebugPart::EDebugHumanoidController) 

enum class EDebugPart;
template<> struct TIsUEnumClass<EDebugPart> { enum { Value = true }; };
template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<EDebugPart>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
