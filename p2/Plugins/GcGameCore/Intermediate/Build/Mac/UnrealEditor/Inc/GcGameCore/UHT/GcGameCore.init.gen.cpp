// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGcGameCore_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_GcGameCore;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_GcGameCore()
	{
		if (!Z_Registration_Info_UPackage__Script_GcGameCore.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/GcGameCore",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x3DC2B659,
				0x81005D0F,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_GcGameCore.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_GcGameCore.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_GcGameCore(Z_Construct_UPackage__Script_GcGameCore, TEXT("/Script/GcGameCore"), Z_Registration_Info_UPackage__Script_GcGameCore, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x3DC2B659, 0x81005D0F));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
