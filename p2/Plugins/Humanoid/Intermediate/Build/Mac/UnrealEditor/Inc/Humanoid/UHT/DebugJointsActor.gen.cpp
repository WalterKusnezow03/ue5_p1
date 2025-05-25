// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Humanoid/Debug/DebugJointsActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDebugJointsActor() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	HUMANOID_API UClass* Z_Construct_UClass_ADebugJointsActor();
	HUMANOID_API UClass* Z_Construct_UClass_ADebugJointsActor_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Humanoid();
// End Cross Module References
	void ADebugJointsActor::StaticRegisterNativesADebugJointsActor()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ADebugJointsActor);
	UClass* Z_Construct_UClass_ADebugJointsActor_NoRegister()
	{
		return ADebugJointsActor::StaticClass();
	}
	struct Z_Construct_UClass_ADebugJointsActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ADebugJointsActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Humanoid,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADebugJointsActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Debug/DebugJointsActor.h" },
		{ "ModuleRelativePath", "Debug/DebugJointsActor.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ADebugJointsActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ADebugJointsActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ADebugJointsActor_Statics::ClassParams = {
		&ADebugJointsActor::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_ADebugJointsActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ADebugJointsActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ADebugJointsActor()
	{
		if (!Z_Registration_Info_UClass_ADebugJointsActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ADebugJointsActor.OuterSingleton, Z_Construct_UClass_ADebugJointsActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ADebugJointsActor.OuterSingleton;
	}
	template<> HUMANOID_API UClass* StaticClass<ADebugJointsActor>()
	{
		return ADebugJointsActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADebugJointsActor);
	ADebugJointsActor::~ADebugJointsActor() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_Humanoid_Source_Humanoid_Debug_DebugJointsActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_Humanoid_Source_Humanoid_Debug_DebugJointsActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ADebugJointsActor, ADebugJointsActor::StaticClass, TEXT("ADebugJointsActor"), &Z_Registration_Info_UClass_ADebugJointsActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ADebugJointsActor), 4004578082U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_Humanoid_Source_Humanoid_Debug_DebugJointsActor_h_2027815303(TEXT("/Script/Humanoid"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_Humanoid_Source_Humanoid_Debug_DebugJointsActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_Humanoid_Source_Humanoid_Debug_DebugJointsActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
