// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/actor/IkDebugActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeIkDebugActor() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	IKHUMANOIDMODELL_API UClass* Z_Construct_UClass_AIkDebugActor();
	IKHUMANOIDMODELL_API UClass* Z_Construct_UClass_AIkDebugActor_NoRegister();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	void AIkDebugActor::StaticRegisterNativesAIkDebugActor()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AIkDebugActor);
	UClass* Z_Construct_UClass_AIkDebugActor_NoRegister()
	{
		return AIkDebugActor::StaticClass();
	}
	struct Z_Construct_UClass_AIkDebugActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AIkDebugActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_IkHumanoidModell,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AIkDebugActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "actor/IkDebugActor.h" },
		{ "ModuleRelativePath", "actor/IkDebugActor.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AIkDebugActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AIkDebugActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AIkDebugActor_Statics::ClassParams = {
		&AIkDebugActor::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AIkDebugActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AIkDebugActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AIkDebugActor()
	{
		if (!Z_Registration_Info_UClass_AIkDebugActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AIkDebugActor.OuterSingleton, Z_Construct_UClass_AIkDebugActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AIkDebugActor.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UClass* StaticClass<AIkDebugActor>()
	{
		return AIkDebugActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AIkDebugActor);
	AIkDebugActor::~AIkDebugActor() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_IkDebugActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_IkDebugActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AIkDebugActor, AIkDebugActor::StaticClass, TEXT("AIkDebugActor"), &Z_Registration_Info_UClass_AIkDebugActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AIkDebugActor), 976201358U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_IkDebugActor_h_2847986970(TEXT("/Script/IkHumanoidModell"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_IkDebugActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_actor_IkDebugActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
