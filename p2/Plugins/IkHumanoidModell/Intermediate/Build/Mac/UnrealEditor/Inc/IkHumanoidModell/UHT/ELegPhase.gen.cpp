// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/Ik/Controller/enums/ELegPhase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeELegPhase() {}
// Cross Module References
	IKHUMANOIDMODELL_API UEnum* Z_Construct_UEnum_IkHumanoidModell_ELegPhase();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ELegPhase;
	static UEnum* ELegPhase_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ELegPhase.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ELegPhase.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_IkHumanoidModell_ELegPhase, (UObject*)Z_Construct_UPackage__Script_IkHumanoidModell(), TEXT("ELegPhase"));
		}
		return Z_Registration_Info_UEnum_ELegPhase.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<ELegPhase>()
	{
		return ELegPhase_StaticEnum();
	}
	struct Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::Enumerators[] = {
		{ "ELegPhase::EForward", (int64)ELegPhase::EForward },
		{ "ELegPhase::EBackward", (int64)ELegPhase::EBackward },
		{ "ELegPhase::EEndInPlace", (int64)ELegPhase::EEndInPlace },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::Enum_MetaDataParams[] = {
		{ "EBackward.Name", "ELegPhase::EBackward" },
		{ "EEndInPlace.Name", "ELegPhase::EEndInPlace" },
		{ "EForward.Name", "ELegPhase::EForward" },
		{ "ModuleRelativePath", "Ik/Controller/enums/ELegPhase.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_IkHumanoidModell,
		nullptr,
		"ELegPhase",
		"ELegPhase",
		Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_IkHumanoidModell_ELegPhase()
	{
		if (!Z_Registration_Info_UEnum_ELegPhase.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ELegPhase.InnerSingleton, Z_Construct_UEnum_IkHumanoidModell_ELegPhase_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ELegPhase.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_ELegPhase_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_ELegPhase_h_Statics::EnumInfo[] = {
		{ ELegPhase_StaticEnum, TEXT("ELegPhase"), &Z_Registration_Info_UEnum_ELegPhase, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2200820847U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_ELegPhase_h_3717267290(TEXT("/Script/IkHumanoidModell"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_ELegPhase_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_ELegPhase_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
