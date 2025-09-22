// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetPlugin/gamestart/assetEnums/staticMeshEnum.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodestaticMeshEnum() {}
// Cross Module References
	ASSETPLUGIN_API UEnum* Z_Construct_UEnum_AssetPlugin_staticMeshEnum();
	UPackage* Z_Construct_UPackage__Script_AssetPlugin();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_staticMeshEnum;
	static UEnum* staticMeshEnum_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_staticMeshEnum.OuterSingleton)
		{
			Z_Registration_Info_UEnum_staticMeshEnum.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_AssetPlugin_staticMeshEnum, (UObject*)Z_Construct_UPackage__Script_AssetPlugin(), TEXT("staticMeshEnum"));
		}
		return Z_Registration_Info_UEnum_staticMeshEnum.OuterSingleton;
	}
	template<> ASSETPLUGIN_API UEnum* StaticEnum<staticMeshEnum>()
	{
		return staticMeshEnum_StaticEnum();
	}
	struct Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::Enumerators[] = {
		{ "staticMeshEnum::grassStaticMesh", (int64)staticMeshEnum::grassStaticMesh },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::Enum_MetaDataParams[] = {
		{ "Comment", "//needed for Template asset tracking.\n" },
		{ "grassStaticMesh.Name", "staticMeshEnum::grassStaticMesh" },
		{ "ModuleRelativePath", "gamestart/assetEnums/staticMeshEnum.h" },
		{ "ToolTip", "needed for Template asset tracking." },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_AssetPlugin,
		nullptr,
		"staticMeshEnum",
		"staticMeshEnum",
		Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_AssetPlugin_staticMeshEnum()
	{
		if (!Z_Registration_Info_UEnum_staticMeshEnum.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_staticMeshEnum.InnerSingleton, Z_Construct_UEnum_AssetPlugin_staticMeshEnum_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_staticMeshEnum.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_staticMeshEnum_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_staticMeshEnum_h_Statics::EnumInfo[] = {
		{ staticMeshEnum_StaticEnum, TEXT("staticMeshEnum"), &Z_Registration_Info_UEnum_staticMeshEnum, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 66081376U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_staticMeshEnum_h_1650287328(TEXT("/Script/AssetPlugin"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_staticMeshEnum_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetPlugin_Source_AssetPlugin_gamestart_assetEnums_staticMeshEnum_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
