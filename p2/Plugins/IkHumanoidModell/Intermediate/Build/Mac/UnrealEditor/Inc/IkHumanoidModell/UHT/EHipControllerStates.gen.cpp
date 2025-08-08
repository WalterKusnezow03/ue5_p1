// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/Ik/Controller/enums/EHipControllerStates.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEHipControllerStates() {}
// Cross Module References
	IKHUMANOIDMODELL_API UEnum* Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EHipControllerStates;
	static UEnum* EHipControllerStates_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EHipControllerStates.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EHipControllerStates.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates, (UObject*)Z_Construct_UPackage__Script_IkHumanoidModell(), TEXT("EHipControllerStates"));
		}
		return Z_Registration_Info_UEnum_EHipControllerStates.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<EHipControllerStates>()
	{
		return EHipControllerStates_StaticEnum();
	}
	struct Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::Enumerators[] = {
		{ "EHipControllerStates::EIdle", (int64)EHipControllerStates::EIdle },
		{ "EHipControllerStates::ELocomotion", (int64)EHipControllerStates::ELocomotion },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::Enum_MetaDataParams[] = {
		{ "EIdle.Name", "EHipControllerStates::EIdle" },
		{ "ELocomotion.Name", "EHipControllerStates::ELocomotion" },
		{ "ModuleRelativePath", "Ik/Controller/enums/EHipControllerStates.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_IkHumanoidModell,
		nullptr,
		"EHipControllerStates",
		"EHipControllerStates",
		Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates()
	{
		if (!Z_Registration_Info_UEnum_EHipControllerStates.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EHipControllerStates.InnerSingleton, Z_Construct_UEnum_IkHumanoidModell_EHipControllerStates_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EHipControllerStates.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_EHipControllerStates_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_EHipControllerStates_h_Statics::EnumInfo[] = {
		{ EHipControllerStates_StaticEnum, TEXT("EHipControllerStates"), &Z_Registration_Info_UEnum_EHipControllerStates, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1911810911U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_EHipControllerStates_h_3766959820(TEXT("/Script/IkHumanoidModell"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_EHipControllerStates_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_enums_EHipControllerStates_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
