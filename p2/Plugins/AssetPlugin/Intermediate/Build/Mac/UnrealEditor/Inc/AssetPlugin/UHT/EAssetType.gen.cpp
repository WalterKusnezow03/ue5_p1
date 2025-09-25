// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/PathMaker/enum/EAssetType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEAssetType() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_EAssetType();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EAssetType;
	static UEnum* EAssetType_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EAssetType.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EAssetType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_EAssetType, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("EAssetType"));
		}
		return Z_Registration_Info_UEnum_EAssetType.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<EAssetType>()
	{
		return EAssetType_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_EAssetType_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::Enumerators[] = {
		{ "EAssetType::EUTexture2D", (int64)EAssetType::EUTexture2D },
		{ "EAssetType::EUClassBlueprint", (int64)EAssetType::EUClassBlueprint },
		{ "EAssetType::EUMaterial", (int64)EAssetType::EUMaterial },
		{ "EAssetType::EUStaticMesh", (int64)EAssetType::EUStaticMesh },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::Enum_MetaDataParams[] = {
		{ "EUClassBlueprint.Name", "EAssetType::EUClassBlueprint" },
		{ "EUMaterial.Name", "EAssetType::EUMaterial" },
		{ "EUStaticMesh.Name", "EAssetType::EUStaticMesh" },
		{ "EUTexture2D.Name", "EAssetType::EUTexture2D" },
		{ "ModuleRelativePath", "gamestart/PathMaker/enum/EAssetType.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"EAssetType",
		"EAssetType",
		Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_EAssetType()
	{
		if (!Z_Registration_Info_UEnum_EAssetType.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EAssetType.InnerSingleton, Z_Construct_UEnum_AssetPlugin_EAssetType_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EAssetType.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_PathMaker_enum_EAssetType_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_PathMaker_enum_EAssetType_h_Statics::EnumInfo[] = {
		{ EAssetType_StaticEnum, TEXT("EAssetType"), &Z_Registration_Info_UEnum_EAssetType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4178765334U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_PathMaker_enum_EAssetType_h_4104537250(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_PathMaker_enum_EAssetType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_PathMaker_enum_EAssetType_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
