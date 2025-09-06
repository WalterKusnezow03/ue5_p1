// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/world/worldLevelBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeworldLevelBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	GAMECORE_API UClass* Z_Construct_UClass_AworldLevelBase();
	GAMECORE_API UClass* Z_Construct_UClass_AworldLevelBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	void AworldLevelBase::StaticRegisterNativesAworldLevelBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AworldLevelBase);
	UClass* Z_Construct_UClass_AworldLevelBase_NoRegister()
	{
		return AworldLevelBase::StaticClass();
	}
	struct Z_Construct_UClass_AworldLevelBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AworldLevelBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_GameCore,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AworldLevelBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * acts as a task interface for plugins - processed in Sub Game Project!\n */" },
		{ "IncludePath", "world/worldLevelBase.h" },
		{ "ModuleRelativePath", "world/worldLevelBase.h" },
		{ "ToolTip", "acts as a task interface for plugins - processed in Sub Game Project!" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AworldLevelBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AworldLevelBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AworldLevelBase_Statics::ClassParams = {
		&AworldLevelBase::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AworldLevelBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AworldLevelBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AworldLevelBase()
	{
		if (!Z_Registration_Info_UClass_AworldLevelBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AworldLevelBase.OuterSingleton, Z_Construct_UClass_AworldLevelBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AworldLevelBase.OuterSingleton;
	}
	template<> GAMECORE_API UClass* StaticClass<AworldLevelBase>()
	{
		return AworldLevelBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AworldLevelBase);
	AworldLevelBase::~AworldLevelBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_world_worldLevelBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_world_worldLevelBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AworldLevelBase, AworldLevelBase::StaticClass, TEXT("AworldLevelBase"), &Z_Registration_Info_UClass_AworldLevelBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AworldLevelBase), 682839573U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_world_worldLevelBase_h_1984751516(TEXT("/Script/GameCore"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_world_worldLevelBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_world_worldLevelBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
