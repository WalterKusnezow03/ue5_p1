// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/carryItems/enum/ECarriedItemPosition.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeECarriedItemPosition() {}
// Cross Module References
	IKHUMANOIDMODELL_API UEnum* Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ECarriedItemPosition;
	static UEnum* ECarriedItemPosition_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ECarriedItemPosition.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ECarriedItemPosition.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition, (UObject*)Z_Construct_UPackage__Script_IkHumanoidModell(), TEXT("ECarriedItemPosition"));
		}
		return Z_Registration_Info_UEnum_ECarriedItemPosition.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<ECarriedItemPosition>()
	{
		return ECarriedItemPosition_StaticEnum();
	}
	struct Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::Enumerators[] = {
		{ "ECarriedItemPosition::EHipPosition", (int64)ECarriedItemPosition::EHipPosition },
		{ "ECarriedItemPosition::ETorsoPosition", (int64)ECarriedItemPosition::ETorsoPosition },
		{ "ECarriedItemPosition::EAimDownSightPosition", (int64)ECarriedItemPosition::EAimDownSightPosition },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::Enum_MetaDataParams[] = {
		{ "EAimDownSightPosition.Name", "ECarriedItemPosition::EAimDownSightPosition" },
		{ "EHipPosition.Name", "ECarriedItemPosition::EHipPosition" },
		{ "ETorsoPosition.Name", "ECarriedItemPosition::ETorsoPosition" },
		{ "ModuleRelativePath", "carryItems/enum/ECarriedItemPosition.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_IkHumanoidModell,
		nullptr,
		"ECarriedItemPosition",
		"ECarriedItemPosition",
		Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition()
	{
		if (!Z_Registration_Info_UEnum_ECarriedItemPosition.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ECarriedItemPosition.InnerSingleton, Z_Construct_UEnum_IkHumanoidModell_ECarriedItemPosition_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ECarriedItemPosition.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_enum_ECarriedItemPosition_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_enum_ECarriedItemPosition_h_Statics::EnumInfo[] = {
		{ ECarriedItemPosition_StaticEnum, TEXT("ECarriedItemPosition"), &Z_Registration_Info_UEnum_ECarriedItemPosition, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 527267827U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_enum_ECarriedItemPosition_h_3798559352(TEXT("/Script/IkHumanoidModell"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_enum_ECarriedItemPosition_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_enum_ECarriedItemPosition_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
