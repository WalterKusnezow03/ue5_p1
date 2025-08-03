// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/actor/debugEnum/EDebugPart.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEDebugPart() {}
// Cross Module References
	IKHUMANOIDMODELL_API UEnum* Z_Construct_UEnum_IkHumanoidModell_EDebugPart();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EDebugPart;
	static UEnum* EDebugPart_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EDebugPart.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EDebugPart.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_IkHumanoidModell_EDebugPart, (UObject*)Z_Construct_UPackage__Script_IkHumanoidModell(), TEXT("EDebugPart"));
		}
		return Z_Registration_Info_UEnum_EDebugPart.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<EDebugPart>()
	{
		return EDebugPart_StaticEnum();
	}
	struct Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::Enumerators[] = {
		{ "EDebugPart::EDebugArm", (int64)EDebugPart::EDebugArm },
		{ "EDebugPart::EDebugLayeredArm", (int64)EDebugPart::EDebugLayeredArm },
		{ "EDebugPart::EDebugTorso", (int64)EDebugPart::EDebugTorso },
		{ "EDebugPart::EDebugHipController", (int64)EDebugPart::EDebugHipController },
		{ "EDebugPart::EDebugHumanoidController", (int64)EDebugPart::EDebugHumanoidController },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::Enum_MetaDataParams[] = {
		{ "EDebugArm.Name", "EDebugPart::EDebugArm" },
		{ "EDebugHipController.Name", "EDebugPart::EDebugHipController" },
		{ "EDebugHumanoidController.Name", "EDebugPart::EDebugHumanoidController" },
		{ "EDebugLayeredArm.Name", "EDebugPart::EDebugLayeredArm" },
		{ "EDebugTorso.Name", "EDebugPart::EDebugTorso" },
		{ "ModuleRelativePath", "actor/debugEnum/EDebugPart.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_IkHumanoidModell,
		nullptr,
		"EDebugPart",
		"EDebugPart",
		Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_IkHumanoidModell_EDebugPart()
	{
		if (!Z_Registration_Info_UEnum_EDebugPart.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EDebugPart.InnerSingleton, Z_Construct_UEnum_IkHumanoidModell_EDebugPart_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EDebugPart.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_debugEnum_EDebugPart_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_debugEnum_EDebugPart_h_Statics::EnumInfo[] = {
		{ EDebugPart_StaticEnum, TEXT("EDebugPart"), &Z_Registration_Info_UEnum_EDebugPart, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 808961947U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_debugEnum_EDebugPart_h_3507358767(TEXT("/Script/IkHumanoidModell"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_debugEnum_EDebugPart_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_debugEnum_EDebugPart_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
