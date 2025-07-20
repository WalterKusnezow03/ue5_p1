// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodecustomUiComponentBase() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UcustomUiComponentBase::StaticRegisterNativesUcustomUiComponentBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UcustomUiComponentBase);
	UClass* Z_Construct_UClass_UcustomUiComponentBase_NoRegister()
	{
		return UcustomUiComponentBase::StaticClass();
	}
	struct Z_Construct_UClass_UcustomUiComponentBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UcustomUiComponentBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UcustomUiComponentBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * base functionality each custom ui element should have \n * (the pointer to the main owning UPlayerUi instance)\n */" },
		{ "IncludePath", "ui/_baseClass/customUiComponentBase.h" },
		{ "ModuleRelativePath", "ui/_baseClass/customUiComponentBase.h" },
		{ "ToolTip", "base functionality each custom ui element should have\n(the pointer to the main owning UPlayerUi instance)" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UcustomUiComponentBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UcustomUiComponentBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UcustomUiComponentBase_Statics::ClassParams = {
		&UcustomUiComponentBase::StaticClass,
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
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UcustomUiComponentBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UcustomUiComponentBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UcustomUiComponentBase()
	{
		if (!Z_Registration_Info_UClass_UcustomUiComponentBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UcustomUiComponentBase.OuterSingleton, Z_Construct_UClass_UcustomUiComponentBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UcustomUiComponentBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UcustomUiComponentBase>()
	{
		return UcustomUiComponentBase::StaticClass();
	}
	UcustomUiComponentBase::UcustomUiComponentBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UcustomUiComponentBase);
	UcustomUiComponentBase::~UcustomUiComponentBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UcustomUiComponentBase, UcustomUiComponentBase::StaticClass, TEXT("UcustomUiComponentBase"), &Z_Registration_Info_UClass_UcustomUiComponentBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UcustomUiComponentBase), 37792784U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_4235032445(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
