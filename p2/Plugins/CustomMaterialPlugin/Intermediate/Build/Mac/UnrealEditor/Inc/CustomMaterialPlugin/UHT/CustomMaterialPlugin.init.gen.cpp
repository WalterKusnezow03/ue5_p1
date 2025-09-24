// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCustomMaterialPlugin_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_CustomMaterialPlugin;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin()
	{
		if (!Z_Registration_Info_UPackage__Script_CustomMaterialPlugin.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/CustomMaterialPlugin",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x8D17DA4E,
				0x36D04D70,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_CustomMaterialPlugin.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_CustomMaterialPlugin.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_CustomMaterialPlugin(Z_Construct_UPackage__Script_CustomMaterialPlugin, TEXT("/Script/CustomMaterialPlugin"), Z_Registration_Info_UPackage__Script_CustomMaterialPlugin, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x8D17DA4E, 0x36D04D70));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
