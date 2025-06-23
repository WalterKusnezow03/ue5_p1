// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Ik/Controller/enums/ELegPhase.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef IKHUMANOIDMODELL_ELegPhase_generated_h
#error "ELegPhase.generated.h already included, missing '#pragma once' in ELegPhase.h"
#endif
#define IKHUMANOIDMODELL_ELegPhase_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_ELegPhase_h


#define FOREACH_ENUM_ELEGPHASE(op) \
	op(ELegPhase::EForward) \
	op(ELegPhase::EBackward) \
	op(ELegPhase::EEndInPlace) 

enum class ELegPhase;
template<> struct TIsUEnumClass<ELegPhase> { enum { Value = true }; };
template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<ELegPhase>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
