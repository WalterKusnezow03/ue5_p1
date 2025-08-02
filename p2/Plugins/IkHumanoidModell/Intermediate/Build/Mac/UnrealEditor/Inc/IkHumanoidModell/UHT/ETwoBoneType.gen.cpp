// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/Ik/Bone/ETwoBoneType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeETwoBoneType() {}
// Cross Module References
	IKHUMANOIDMODELL_API UEnum* Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ETwoBoneType;
	static UEnum* ETwoBoneType_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ETwoBoneType.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ETwoBoneType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType, (UObject*)Z_Construct_UPackage__Script_IkHumanoidModell(), TEXT("ETwoBoneType"));
		}
		return Z_Registration_Info_UEnum_ETwoBoneType.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UEnum* StaticEnum<ETwoBoneType>()
	{
		return ETwoBoneType_StaticEnum();
	}
	struct Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::Enumerators[] = {
		{ "ETwoBoneType::ETwoBoneLeg", (int64)ETwoBoneType::ETwoBoneLeg },
		{ "ETwoBoneType::ETwoBoneArm", (int64)ETwoBoneType::ETwoBoneArm },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::Enum_MetaDataParams[] = {
		{ "ETwoBoneArm.Name", "ETwoBoneType::ETwoBoneArm" },
		{ "ETwoBoneLeg.Name", "ETwoBoneType::ETwoBoneLeg" },
		{ "ModuleRelativePath", "Ik/Bone/ETwoBoneType.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_IkHumanoidModell,
		nullptr,
		"ETwoBoneType",
		"ETwoBoneType",
		Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType()
	{
		if (!Z_Registration_Info_UEnum_ETwoBoneType.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ETwoBoneType.InnerSingleton, Z_Construct_UEnum_IkHumanoidModell_ETwoBoneType_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ETwoBoneType.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Bone_ETwoBoneType_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Bone_ETwoBoneType_h_Statics::EnumInfo[] = {
		{ ETwoBoneType_StaticEnum, TEXT("ETwoBoneType"), &Z_Registration_Info_UEnum_ETwoBoneType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1290295970U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Bone_ETwoBoneType_h_323138787(TEXT("/Script/IkHumanoidModell"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Bone_ETwoBoneType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_Ik_Bone_ETwoBoneType_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
