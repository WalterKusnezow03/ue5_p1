// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/baseInterface/BaseUiInterface.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBaseUiInterface() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UBaseUiInterface();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UBaseUiInterface_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UBaseUiInterface::StaticRegisterNativesUBaseUiInterface()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBaseUiInterface);
	UClass* Z_Construct_UClass_UBaseUiInterface_NoRegister()
	{
		return UBaseUiInterface::StaticClass();
	}
	struct Z_Construct_UClass_UBaseUiInterface_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UBaseUiInterface_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UInterface,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UBaseUiInterface_Statics::Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "baseInterface/BaseUiInterface.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UBaseUiInterface_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IBaseUiInterface>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UBaseUiInterface_Statics::ClassParams = {
		&UBaseUiInterface::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001840A1u,
		METADATA_PARAMS(Z_Construct_UClass_UBaseUiInterface_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UBaseUiInterface_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UBaseUiInterface()
	{
		if (!Z_Registration_Info_UClass_UBaseUiInterface.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBaseUiInterface.OuterSingleton, Z_Construct_UClass_UBaseUiInterface_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UBaseUiInterface.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UBaseUiInterface>()
	{
		return UBaseUiInterface::StaticClass();
	}
	UBaseUiInterface::UBaseUiInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UBaseUiInterface);
	UBaseUiInterface::~UBaseUiInterface() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UBaseUiInterface, UBaseUiInterface::StaticClass, TEXT("UBaseUiInterface"), &Z_Registration_Info_UClass_UBaseUiInterface, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBaseUiInterface), 2471973962U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_2365546393(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
