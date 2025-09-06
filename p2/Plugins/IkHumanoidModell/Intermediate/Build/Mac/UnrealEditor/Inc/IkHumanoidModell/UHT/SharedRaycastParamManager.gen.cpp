// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/SharedRaycastParams/SharedRaycastParamManager.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSharedRaycastParamManager() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	IKHUMANOIDMODELL_API UClass* Z_Construct_UClass_ASharedRaycastParamManager();
	IKHUMANOIDMODELL_API UClass* Z_Construct_UClass_ASharedRaycastParamManager_NoRegister();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	void ASharedRaycastParamManager::StaticRegisterNativesASharedRaycastParamManager()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ASharedRaycastParamManager);
	UClass* Z_Construct_UClass_ASharedRaycastParamManager_NoRegister()
	{
		return ASharedRaycastParamManager::StaticClass();
	}
	struct Z_Construct_UClass_ASharedRaycastParamManager_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASharedRaycastParamManager_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_IkHumanoidModell,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASharedRaycastParamManager_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief will track all skelletons and items in a collision list and \n/// automatically make an instance of this object if not created for this world yet\n/// will share all collision params of all skelleton limbs and items\n" },
		{ "IncludePath", "SharedRaycastParams/SharedRaycastParamManager.h" },
		{ "ModuleRelativePath", "SharedRaycastParams/SharedRaycastParamManager.h" },
		{ "ToolTip", "@brief will track all skelletons and items in a collision list and\nautomatically make an instance of this object if not created for this world yet\nwill share all collision params of all skelleton limbs and items" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASharedRaycastParamManager_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASharedRaycastParamManager>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ASharedRaycastParamManager_Statics::ClassParams = {
		&ASharedRaycastParamManager::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_ASharedRaycastParamManager_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ASharedRaycastParamManager_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASharedRaycastParamManager()
	{
		if (!Z_Registration_Info_UClass_ASharedRaycastParamManager.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ASharedRaycastParamManager.OuterSingleton, Z_Construct_UClass_ASharedRaycastParamManager_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ASharedRaycastParamManager.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UClass* StaticClass<ASharedRaycastParamManager>()
	{
		return ASharedRaycastParamManager::StaticClass();
	}
	ASharedRaycastParamManager::ASharedRaycastParamManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASharedRaycastParamManager);
	ASharedRaycastParamManager::~ASharedRaycastParamManager() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_SharedRaycastParams_SharedRaycastParamManager_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_SharedRaycastParams_SharedRaycastParamManager_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ASharedRaycastParamManager, ASharedRaycastParamManager::StaticClass, TEXT("ASharedRaycastParamManager"), &Z_Registration_Info_UClass_ASharedRaycastParamManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ASharedRaycastParamManager), 1959474003U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_SharedRaycastParams_SharedRaycastParamManager_h_2581865170(TEXT("/Script/IkHumanoidModell"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_SharedRaycastParams_SharedRaycastParamManager_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_SharedRaycastParams_SharedRaycastParamManager_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
