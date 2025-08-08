// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Ik/Controller/enums/EHipControllerStates.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef IKHUMANOIDMODELL_EHipControllerStates_generated_h
#error "EHipControllerStates.generated.h already included, missing '#pragma once' in EHipControllerStates.h"
#endif
#define IKHUMANOIDMODELL_EHipControllerStates_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_EHipControllerStates_h


#define FOREACH_ENUM_EHIPCONTROLLERSTATES(op) \
	op(EHipControllerStates::EIdle) \
	op(EHipControllerStates::ELocomotion) 

enum class EHipControllerStates;
template<> struct TIsUEnumClass<EHipControllerStates> { enum { Value = true }; };
template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<EHipControllerStates>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
