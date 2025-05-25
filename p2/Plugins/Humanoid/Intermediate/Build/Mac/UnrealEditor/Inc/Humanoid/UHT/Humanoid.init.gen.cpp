// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeHumanoid_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Humanoid;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Humanoid()
	{
		if (!Z_Registration_Info_UPackage__Script_Humanoid.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Humanoid",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x1B9C6E11,
				0x7801BEA6,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Humanoid.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Humanoid.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Humanoid(Z_Construct_UPackage__Script_Humanoid, TEXT("/Script/Humanoid"), Z_Registration_Info_UPackage__Script_Humanoid, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x1B9C6E11, 0x7801BEA6));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
