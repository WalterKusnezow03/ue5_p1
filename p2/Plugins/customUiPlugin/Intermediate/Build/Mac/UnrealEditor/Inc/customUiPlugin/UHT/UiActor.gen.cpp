// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/TickUpdate/UiActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUiActor() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_AUiActor();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_AUiActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void AUiActor::StaticRegisterNativesAUiActor()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AUiActor);
	UClass* Z_Construct_UClass_AUiActor_NoRegister()
	{
		return AUiActor::StaticClass();
	}
	struct Z_Construct_UClass_AUiActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AUiActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AUiActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "ui/TickUpdate/UiActor.h" },
		{ "ModuleRelativePath", "ui/TickUpdate/UiActor.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AUiActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AUiActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AUiActor_Statics::ClassParams = {
		&AUiActor::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AUiActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AUiActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AUiActor()
	{
		if (!Z_Registration_Info_UClass_AUiActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AUiActor.OuterSingleton, Z_Construct_UClass_AUiActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AUiActor.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<AUiActor>()
	{
		return AUiActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AUiActor);
	AUiActor::~AUiActor() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_TickUpdate_UiActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_TickUpdate_UiActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AUiActor, AUiActor::StaticClass, TEXT("AUiActor"), &Z_Registration_Info_UClass_AUiActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AUiActor), 3069089451U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_TickUpdate_UiActor_h_1613344305(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_TickUpdate_UiActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_TickUpdate_UiActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
