// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEArmType() {}
// Cross Module References
	IKHUMANOIDMODELL_API UEnum* Z_Construct_UEnum_IkHumanoidModell_EArmType();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EArmType;
	static UEnum* EArmType_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EArmType.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EArmType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_IkHumanoidModell_EArmType, (UObject*)Z_Construct_UPackage__Script_IkHumanoidModell(), TEXT("EArmType"));
		}
		return Z_Registration_Info_UEnum_EArmType.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<EArmType>()
	{
		return EArmType_StaticEnum();
	}
	struct Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::Enumerators[] = {
		{ "EArmType::ELeft", (int64)EArmType::ELeft },
		{ "EArmType::ERight", (int64)EArmType::ERight },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::Enum_MetaDataParams[] = {
		{ "ELeft.Name", "EArmType::ELeft" },
		{ "ERight.Name", "EArmType::ERight" },
		{ "ModuleRelativePath", "Ik/Controller/TorsoController/EArmType.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_IkHumanoidModell,
		nullptr,
		"EArmType",
		"EArmType",
		Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_IkHumanoidModell_EArmType()
	{
		if (!Z_Registration_Info_UEnum_EArmType.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EArmType.InnerSingleton, Z_Construct_UEnum_IkHumanoidModell_EArmType_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EArmType.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_TorsoController_EArmType_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_TorsoController_EArmType_h_Statics::EnumInfo[] = {
		{ EArmType_StaticEnum, TEXT("EArmType"), &Z_Registration_Info_UEnum_EArmType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3355707564U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_TorsoController_EArmType_h_2961883075(TEXT("/Script/IkHumanoidModell"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_TorsoController_EArmType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Controller_TorsoController_EArmType_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
